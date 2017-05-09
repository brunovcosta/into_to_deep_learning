images_file = File.open "./train-images-idx3-ubyte", "rb"
magic_number = images_file.read(4).unpack("N").first
if magic_number != 2051
	puts magic_number.to_s 2
	puts magic_number
	raise "Arquivo corrompido"
end

images_number = images_fil.read(4).unpack("N").first

for t in 1..images_number

end

