package com.example.hellonnapi

import android.annotation.SuppressLint
import android.databinding.DataBindingUtil
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.example.hellonnapi.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    // We don't care about localization here
    @SuppressLint("SetTextI18n")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = DataBindingUtil.setContentView(this, R.layout.activity_main)

        binding.button.setOnClickListener {
            binding.textView.text = "Inference result = ${helloNnapi()}"
        }
    }

    external fun helloNnapi(): Float

    companion object {
        init {
            System.loadLibrary("hellonnapi")
        }
    }
}