# frozen_string_literal: true

MODEL = "vosk-model-small-en-us-0.15"

# Vosk.set_log_level(-1)

model      = Vosk::Model.new(MODEL)
recognizer = Vosk::Recognizer.new(model, 16_000.0)

file = File.open("../python/example/test.wav", "r")
file.seek 44, IO::SEEK_SET

audio = file.read.unpack("c*") # char *

puts recognizer.accept_waveform(audio)

result = recognizer.final_result

# extract text from JSON
#
# ```
# {
#   "text": "..."
# }
# ```
#
transcription = result[14...-3]

puts transcription
