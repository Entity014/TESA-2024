def takebit() :
    file_name = "Day2.wav" #file name
    file_loc = 'C:/Users/Glock/Desktop/TESA-2024/Server/socket/socket-back/fileconvert/' + file_name        #path string
    
    with open(file_loc, 'rb') as file_data:
        data = file_data.read()
    return data


def create_file_from_binary(data, output_file_name):
    # Open the file in write-binary mode ('wb')
    with open(output_file_name, 'wb') as file:
        file.write(data)  # Write the binary data to the file
    print(f"File '{output_file_name}' created successfully.")

data = takebit()

create_file_from_binary(data,'new_bearXD.wav')