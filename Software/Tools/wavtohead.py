with open("output.wav", "rb") as f:
    data = f.read()

with open("one.h", "w") as f:
    f.write("#ifndef ONE_WAV_H\n#define ONE_WAV_H\n\n")
    f.write("const unsigned char one_wav[] = {\n")
    
    for i, byte in enumerate(data):
        f.write(f" 0x{byte:02x},")
        if (i + 1) % 12 == 0:
            f.write("\n")
    
    f.write("\n};\n")
    f.write(f"const unsigned int one_wav_len = {len(data)};\n\n")
    f.write("#endif // ONE_WAV_H\n")

print("Generated one.h successfully!")
