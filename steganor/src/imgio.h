#ifndef IMGIO_H
#define IMGIO_H

#include <stddef.h>

/** 
 * \brief Pozice prvního bytu oblasti obrazových dat, tedy (BMP_V3_DATA_START - 1) je poslední byte hlavičky.
 *        Toto číslo je dáno verzí standardu bmp formátu, který obrázky ve složce `data` používají (V3).
 *        Více informací viz např. https://en.wikipedia.org/wiki/BMP_file_format (jenom pro zajímavost).
*/
#define BMP_V3_DATA_START 56

/**
 * \brief Načte data obrázku (včetně hlavičky) ze souboru `path`. Obsah souboru bude uložen do dynamicky
 *        alokované paměti. V případě úspěchu bude ukazatel na incializovanou dynamicky alokovanou paměť
 *        uložen na adresu `image_data` a funkce vrátí velikost načtených dat.
 * \param path Cesta k načítanému obrazovému souboru.
 * \param image_data Ukazatel na inicializovaný ukazatel. Při neúspěchu odkazovaný ukazatel nebude změněn.
 * \return size_t Počet načtených bytů obrazových dat, nebo 0 při chybě.
 */
size_t load_image(const char *path, char **image_data); 

/**
 * \brief Funkce zapíše zadaná obrazová data `image_data` do souboru `path`.
 * \param path Cesta k výstupnímu souboru, kam budou data `image_data` zapsána.
 * \param image_data Obrazová data, která budou do souboru `path` zapsána.
 * \param size Velikost obrazových dat v bytech.
 * \return int 1 při úspěchu, jinak 0.
 */
int save_image(const char *path, const char *image_data, const size_t size);

#endif
