@echo off
#for %%N in (1 2 3 4 5 6 7 8 9 10 20 30 40 50 60 70 80 90) do (
#    ffmpeg -i %%N.wav -acodec pcm_s16le -ar 16000 -ac 1 %%N_1.wav
#)
#
#ffmpeg -i temp.wav -acodec pcm_s16le -ar 16000 -ac 1 temp_1.wav
#ffmpeg -i humid.wav -acodec pcm_s16le -ar 16000 -ac 1 humid_1.wav
#ffmpeg -i point.wav -acodec pcm_s16le -ar 16000 -ac 1 point_1.wav
ffmpeg -i 0_1.wav -acodec pcm_s16le -ar 16000 -ac 1 0.wav
