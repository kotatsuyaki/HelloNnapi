package com.example.hellonnapi

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material.Button
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.tooling.preview.PreviewParameter
import androidx.compose.ui.tooling.preview.PreviewParameterProvider
import androidx.compose.ui.unit.dp
import com.example.hellonnapi.ui.theme.HelloNnapiTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            var answerText by remember { mutableStateOf("Waiting for inference") };
            MainActivityContent(answerText) {
                Log.v("MainActivity", "Inference callback called")
                answerText = "Inference result: ${helloNnapi()}"
            }
        }
    }

    external fun helloNnapi(): Float

    companion object {
        init {
            System.loadLibrary("hellonnapi")
        }
    }
}

class DummyStringPreviewProvider: PreviewParameterProvider<String> {
    override val values = sequenceOf("Waiting for inference", "Inference result: 42.24")
}

class DummyCallbackPreviewProvider: PreviewParameterProvider<() -> Unit> {
    override val values = sequenceOf({})
}

@Preview(showBackground = true)
@Composable
fun MainActivityContent(
    @PreviewParameter(DummyStringPreviewProvider::class) answerText: String,
    onClick: () -> Unit = {},
) {
    HelloNnapiTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = MaterialTheme.colors.background,
        ) {
            Column(
                modifier = Modifier.fillMaxSize(),
                horizontalAlignment = Alignment.CenterHorizontally,
                verticalArrangement = Arrangement.spacedBy(
                    15.dp,
                    Alignment.CenterVertically
                ),
            ) {
                Text(text = answerText)
                Button(onClick = {
                    onClick()
                }) {
                    Text(text = "run inference".uppercase())
                }
            }
        }
    }
}
