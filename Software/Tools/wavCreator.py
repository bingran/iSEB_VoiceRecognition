from gtts import gTTS

# Create a TTS object for "one"
tts = gTTS("zero", lang="en")

# Save the audio file
tts.save("0_1.wav")