#include <stdio.h>
#include <cs50.h>
#include <stdint.h>
//#include "bmp.h"

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

int main(int argc, char *argv[])
{


    // open the card.raw file and check for NULL
    FILE *card_ptr = fopen("card.raw", "r");

    if (card_ptr == NULL)
    {
        fprintf(stderr, "File not found!");
        return 1;
    }

    //read in 512 bytes from card.raw until you reach EOF (end byte is 0)
    BYTE buffer[512];
    bool found_first_jpg = false;
    FILE *new_jpg_ptr;
    int file_counter = 0;
    while (fread(buffer, 1, 512, card_ptr) != 0x00)
    {
        //if we find the header pattern of bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if we have found the first jpg
            if (!found_first_jpg)
            {
                //change the boolean value
                found_first_jpg = true;
                //write in the bytes  into a separate file
                //1)open the file
                char filename[8];
                sprintf(filename, "%03i.jpg", file_counter++);
                new_jpg_ptr = fopen(filename, "w");
                if (new_jpg_ptr == NULL)
                {
                    return 2;
                }

                //2) write from buffer into new_jpg_ptr
                fwrite(buffer, 1, 512, new_jpg_ptr);
            }
            else
            {
                //close the previous file
                fclose(new_jpg_ptr);

                //write in the bytes into a new files

                //write in the bytes  into a separate file
                //1)open the file
                char filename[8];
                sprintf(filename, "%03i.jpg", file_counter++);
                new_jpg_ptr = fopen(filename, "w");
                if (new_jpg_ptr == NULL)
                {
                    return 3;
                }

                //2) write from buffer into new_jpg_ptr
                fwrite(buffer, 1, 512, new_jpg_ptr);
            }

        }

        else
        {
            //if we had found the first jpg
            if (found_first_jpg)
            {
                // countinue to write the bytes
                fwrite(buffer, 1, 512, new_jpg_ptr);
            }

        }


    }
    //close all files and free the memory used
    fclose(new_jpg_ptr);
    fclose(card_ptr);

    return 0;
}
