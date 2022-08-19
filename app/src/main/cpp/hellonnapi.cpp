#include <android/NeuralNetworks.h>
#include <android/log.h>
#include <jni.h>

#include <array>
#include <cassert>

const char *APP_NAME = "HelloNnapi";
#define CHECK_NN_NO_ERROR(RETURN_VALUE)                                        \
  if ((RETURN_VALUE) != ANEURALNETWORKS_NO_ERROR) {                            \
    __android_log_print(ANDROID_LOG_WARN, APP_NAME,                            \
                        "NNAPI function returned an error: %d", RETURN_VALUE); \
    assert(false);                                                             \
  }

namespace {
ANeuralNetworksOperandType createScalarOperandType(int32_t type) {
  ANeuralNetworksOperandType operand_type;
  operand_type.type = type;
  operand_type.dimensionCount = 0;
  operand_type.dimensions = nullptr;
  operand_type.scale = 0.0f;
  operand_type.zeroPoint = 0;
  return operand_type;
}
}  // namespace

using std::array;
extern "C" JNIEXPORT jfloat JNICALL
Java_com_example_hellonnapi_MainActivity_helloNnapi(JNIEnv *env, jobject thiz) {
  const auto FLOAT_SCALAR_OPERAND_TYPE =
      createScalarOperandType(ANEURALNETWORKS_TENSOR_FLOAT32);
  const auto INT_SCALAR_OPERAND_TYPE =
      createScalarOperandType(ANEURALNETWORKS_TENSOR_INT32);
  uint32_t nextOperand = 0;

  // Create model
  ANeuralNetworksModel *model = nullptr;
  CHECK_NN_NO_ERROR(ANeuralNetworksModel_create(&model))

  // Create input operands
  const uint32_t aOperandId = nextOperand++;
  CHECK_NN_NO_ERROR(
      ANeuralNetworksModel_addOperand(model, &FLOAT_SCALAR_OPERAND_TYPE))

  const uint32_t bOperandId = nextOperand++;
  CHECK_NN_NO_ERROR(
      ANeuralNetworksModel_addOperand(model, &FLOAT_SCALAR_OPERAND_TYPE))

  const uint32_t fuseCodeOperandId = nextOperand++;
  CHECK_NN_NO_ERROR(
      ANeuralNetworksModel_addOperand(model, &INT_SCALAR_OPERAND_TYPE));

  const auto fuseCode = ANEURALNETWORKS_FUSED_NONE;
  ANeuralNetworksModel_setOperandValue(model,
                                       static_cast<int32_t>(fuseCodeOperandId),
                                       &fuseCode, sizeof fuseCode);

  // Create output operand
  const uint32_t outOperandId = nextOperand++;
  CHECK_NN_NO_ERROR(
      ANeuralNetworksModel_addOperand(model, &FLOAT_SCALAR_OPERAND_TYPE));

  // Add operation
  const array operationInputs{aOperandId, bOperandId, fuseCodeOperandId};
  const array operationOutputs{outOperandId};
  CHECK_NN_NO_ERROR(ANeuralNetworksModel_addOperation(
      model, ANEURALNETWORKS_ADD, operationInputs.size(),
      operationInputs.data(), operationOutputs.size(),
      operationOutputs.data()));

  // Identify I/O of the model
  const array modelInputs{aOperandId, bOperandId};
  const array modelOutputs{outOperandId};
  CHECK_NN_NO_ERROR(ANeuralNetworksModel_identifyInputsAndOutputs(
      model, modelInputs.size(), modelInputs.data(), modelOutputs.size(),
      modelOutputs.data()));

  // Finish model
  CHECK_NN_NO_ERROR(ANeuralNetworksModel_finish(model));

  // Compile
  ANeuralNetworksCompilation *compilation = nullptr;
  CHECK_NN_NO_ERROR(ANeuralNetworksCompilation_create(model, &compilation));

  CHECK_NN_NO_ERROR(ANeuralNetworksCompilation_setPreference(
      compilation, ANEURALNETWORKS_PREFER_FAST_SINGLE_ANSWER));

  CHECK_NN_NO_ERROR(ANeuralNetworksCompilation_finish(compilation));

  // Execute
  ANeuralNetworksExecution *run = nullptr;
  CHECK_NN_NO_ERROR(ANeuralNetworksExecution_create(compilation, &run));

  const float inputA = 42.0f;
  const float inputB = 0.42f;
  float output = 0.0f;
  CHECK_NN_NO_ERROR(ANeuralNetworksExecution_setInput(run, 0, nullptr, &inputA,
                                                      sizeof(float)));
  CHECK_NN_NO_ERROR(ANeuralNetworksExecution_setInput(run, 1, nullptr, &inputB,
                                                      sizeof(float)));
  CHECK_NN_NO_ERROR(ANeuralNetworksExecution_setOutput(run, 0, nullptr, &output,
                                                       sizeof(float)));

  ANeuralNetworksEvent *event = nullptr;
  CHECK_NN_NO_ERROR(ANeuralNetworksExecution_startCompute(run, &event));

  CHECK_NN_NO_ERROR(ANeuralNetworksEvent_wait(event));
  ANeuralNetworksEvent_free(event);
  ANeuralNetworksExecution_free(run);

  return output;
}