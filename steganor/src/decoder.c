#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "imgio.h"

/**
 * \brief Funkce dekóduje zprávu, která je ukryta v poli `img_data`. Způsob ukrytí zprávy bude vysvětlen
 *        vyučujícím. Ve zkratce lze říci, že zpráva je složena z LSB bitů obrazových dat, které v této 
 *        verzi formátu BMP (BITMAPV3INFOHEADER) začínají na bajtu BMP_V3_DATA_START. Konec zprávy je
 *        indikován standardně ukončovací nulou.
 * \param img_data Obrazová data, ve kterých je ukryta zpráva.
 * \param size Velikost obrazových dat.
 */
void decode_message(const char *img_data, const size_t size) {
    int offset = 8;
    char *start_pointer = *img_data + BMP_V3_DATA_START + offset;
    char temp;
    int iterator = 0;

    for(;;){
        temp = ((uint16_t)*start_pointer >> 8) & 0xFF; //8bitu
        if(temp != '\0'){
            // Create a mask with only the n-th bit set to 1
            unsigned int mask = 1 << offset;

            // Extract the bit using AND and right shift
            unsigned int extracted_bit = (*temp & mask) >> offset;

            temp = temp | extracted_bit << iterator;

            iterator += iterator;
            *start_pointer + offset;
        }
        else break;
    }

    printf("%s", temp);
}

/**
 * \brief Hlavní přístupový bod aplikace. Funkce načte obrazová data ze souboru zadaného argumenty
 *        programu. Načtená data jsou předána funkci `decode_message`, která skrytou informaci 
 *        extrahuje a rovnou vypíše.
 * \param argc Počet argumentů aplikace.
 * \param argv Argumenty aplikace.
 * \return int EXIT_SUCCESS při úspěchu, jinak EXIT_FAILURE.
 */
int main(int argc, char *argv[]) {
    size_t image_size;
    char *image_data;

    if (argc < 2) {
        puts("steganodecoder <input-bmp-img-file>");
        return EXIT_FAILURE;
    }

    image_size = load_image(argv[1], &image_data);
    if (image_size == 0) {
        return EXIT_FAILURE;
    }

    decode_message(image_data, image_size);
    return EXIT_SUCCESS;
}