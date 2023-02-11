from console_gfx import ConsoleGfx


def decimal_to_hex(digit):
    decimal = ''
    if digit < 10:
        decimal = str(digit)
    elif digit == 10:
        decimal = 'a'
    elif digit == 11:
        decimal = 'b'
    elif digit == 12:
        decimal = 'c'
    elif digit == 13:
        decimal = 'd'
    elif digit == 14:
        decimal = 'e'
    elif digit == 15:
        decimal = 'f'

    return decimal


def hex_to_decimal(digit):
    decimal = 0
    if digit.isdigit():
        decimal = int(digit)
    elif digit == "A" or digit == "a":
        decimal = 10
    elif digit == "B" or digit == "b":
        decimal = 11
    elif digit == "C" or digit == "c":
        decimal = 12
    elif digit == "D" or digit == "d":
        decimal = 13
    elif digit == "E" or digit == "e":
        decimal = 14
    elif digit == "F" or digit == "f":
        decimal = 15

    return decimal


def menu():
    print("\nRLE Menu\n"
          "--------")
    print("0. Exit")
    print("1. Load File")
    print("2. Load Test Image")
    print("3. Read RLE String")
    print("4. Read RLE Hex String")
    print("5. Read Data Hex String")
    print("6. Display Image")
    print("7. Display RLE String")
    print("8. Display Hex RLE Data")
    print("9. Display Hex Flat Data\n")


# Function 1

def to_hex_string(data):  # DONE
    hex_string = ''
    # i goes through the data (list), if that number in data is <10, number is added to string
    # if that number is >10, the respective letter is added, Ex: 5->5, 11->B
    for i in data:
        if i < 10:
            hex_string = hex_string + str(i)
        elif i == 10:
            hex_string = hex_string + 'a'
        elif i == 11:
            hex_string = hex_string + 'b'
        elif i == 12:
            hex_string = hex_string + 'c'
        elif i == 13:
            hex_string = hex_string + 'd'
        elif i == 14:
            hex_string = hex_string + 'e'
        elif i == 15:
            hex_string = hex_string + 'f'
            # DONE
    return hex_string


# Function 2

def count_runs(flat_data):
    count = 1

    if not flat_data:
        return 0

    temp = flat_data[0]

    runs = 0

    # [15, 15, 15, 4, 4, 4, 4, 4, 4]
    for i in range(len(flat_data)):
        if temp != flat_data[i]:
            count = 0
            runs += 1
            temp = flat_data[i]
        if count == 15:
            count = 0
            runs += 1
        count += 1

    return runs + 1


# Function 3

def encode_rle(flat_data):  # DONE
    results = []
    count = 0
    temp = flat_data[0]
    # iterate through flat_data
    for i in range(len(flat_data)):
        if temp == flat_data[i]:
            count += 1
            # if repeated increment count by 1
            if count == 15:
                results.append(count)
                results.append(temp)
                count = 0
                temp = flat_data[i]
        else:
            results.append(count)
            results.append(temp)
            count = 1
            temp = flat_data[i]

    if count != 15:
        results.append(count)
        results.append(temp)
    return results


# [4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 7]
#  0  1  2  3  4 5            10                            10                            10
# print((encode_rle('1, 3, 4, 4, 5, 6, 6'))) DONE

# Function 4

def get_decoded_length(rle_data):
    length = 0
    for i in rle_data[::2]:
        length += i
    return length


# Function 5

def decode_rle(rle_data):
    result = []
    # [3, 15, 6, 4]
    #  0  1   2  3

    # iterates through values that reps pixels (0-15)
    for i in range(1, len(rle_data), 2):
        # iterates through nums that reps how many times a pixel is repeated
        for j in range(rle_data[i - 1]):
            result.append(rle_data[i])
    return result


# print(decode_rle('3, 15, 6, 4')) DONE


# Function 6

def string_to_data(data_string):
    data_list = []
    for i in range(0, len(data_string)):
        if data_string[i].isdigit():
            ans = int(data_string[i])
            data_list.append(ans)
        elif data_string[i] == 'a' or data_string[i] == 'A':
            ans = 10
            data_list.append(ans)
        elif data_string[i] == 'b' or data_string[i] == 'B':
            ans = 11
            data_list.append(ans)
            # Respective hex values converted to deci
        elif data_string[i] == 'c' or data_string[i] == 'C':
            ans = 12
            data_list.append(ans)
        elif data_string[i] == 'd' or data_string[i] == 'D':
            ans = 13
            data_list.append(ans)
        elif data_string[i] == 'e' or data_string[i] == 'E':
            ans = 14
            data_list.append(ans)
        elif data_string[i] == 'f' or data_string[i] == 'F':
            ans = 15
            data_list.append(ans)
    return data_list


def to_rle_string(rle_data):
    rle_string = ''
    # [15, 15, 6, 4]
    for i in range(0, len(rle_data), 2):
        rle_string = rle_string + str(rle_data[i]) + decimal_to_hex(rle_data[i + 1]) + ':'
    # print("BEFORE:", rle_string)
    rle_string = rle_string[0:len(rle_string) - 1]
    # print("AFTER:",rle_string)
    return rle_string


def string_to_rle(rle_string):
    num_colons = 0

    rle_list = []
    for i in rle_string:
        if i == ':':
            num_colons += 1

    for i in range(num_colons + 1):
        index = rle_string.find(':')  # find the first colon in the string: rle_string

        if index != -1:  # if there are multiple colons in the string
            rle_sub_string = rle_string[:index]
            rle_string = rle_string[index + 1:]
        else:
            rle_sub_string = rle_string

        if len(rle_sub_string) == 3:
            rle_list.append(int(rle_sub_string[:2]))
            rle_list.append(hex_to_decimal(rle_sub_string[2]))
        else:
            rle_list.append(int(rle_sub_string[:1]))
            rle_list.append(hex_to_decimal(rle_sub_string[1]))

    return rle_list


if __name__ == '__main__':

    decoded = ''
    image_data = None

    print("Welcome to the RLE image encoder!\n")
    print("Displaying Spectrum Image:")
    ConsoleGfx.display_image(ConsoleGfx.test_rainbow)
    print("\n\n")

    running = True

    while running:
        menu()
        menu_option = int(input("Select a Menu Option: "))  # user input for menu options

        if menu_option == 1:  # load image

            filename = input("Enter name of file to load: ")
            image_data = ConsoleGfx.load_file(filename)

        elif menu_option == 2:  # load test image

            print("Test image data loaded.\n")

            image_data = ConsoleGfx.test_image

        elif menu_option == 3:  # Reads RLE data from the user in decimal notation with delimiters (smiley example):
            RLE_decoded = input("Enter an RLE string to be decoded: ", )
            list_str = string_to_rle(RLE_decoded)
            decoded = decode_rle(list_str)


        elif menu_option == 4:  # Reads RLE data from the user in hexadecimal notation without delimiters
            rle_hex_string = input("Enter the hex string holding RLE data: ", )
            list_std = string_to_data(rle_hex_string)
            decoded = decode_rle(list_std)


        elif menu_option == 5:  # Reads raw (flat) data from the user in hexadecimal notation (smiley example)
            hex_string_flat = input(f"Enter the hex string holding flat data: ", )
            decoded = string_to_data(hex_string_flat)

        elif menu_option == 6:  # display loaded image

            print("Displaying image...\n")

            (ConsoleGfx.display_image(image_data))

        elif menu_option == 7:
            print("RLE representation:", to_rle_string(encode_rle(decoded)))

        elif menu_option == 8:
            print("RLE hex values:", to_hex_string(encode_rle(decoded)))

        elif menu_option == 9:
            print("Flat hex values:", to_hex_string(decoded))

        elif menu_option == 0:
            break
