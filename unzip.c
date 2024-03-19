#include <stdio.h>
#include <stdlib.h>
#include <zip.h>

int main() {
    char zip_path[256];
    printf("Veuillez entrer le chemin du fichier ZIP à décompresser : ");
    scanf("%255s", zip_path); 
    
    
    int err = 0;
    zip_t *z = zip_open(zip_path, 0, &err);

    if (z == NULL) 
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier ZIP.\n");
        return 1;
    }

    
    int num_entries = zip_get_num_entries(z, 0);
    if (num_entries < 0) 
    {
        fprintf(stderr, "Error retrieving number of entries in ZIP file.\n");
        zip_close(z);
        return 1;
    }

    
    for (int i = 0; i < num_entries; i++) 
    {
       
        struct zip_stat st;
        zip_stat_index(z, i, 0, &st);

        
        zip_file_t *f = zip_fopen_index(z, i, 0);
        if (f == NULL) 
        {
            fprintf(stderr, "Error opening entry %d in ZIP file.\n", i);
            zip_close(z);
            return 1;
        }

        
        FILE *output_file = fopen(st.name, "wb");
        if (output_file == NULL) 
        {
            fprintf(stderr, "Error creating output file for input %d.\n", i);
            zip_fclose(f);
            zip_close(z);
            return 1;
        }

       
        char buf[1024];
        zip_int64_t nbytes;
        while ((nbytes = zip_fread(f, buf, sizeof(buf))) > 0) 
        {
            fwrite(buf, 1, nbytes, output_file);
        }

       
        fclose(output_file);
        zip_fclose(f);
    }

    // Fermer le fichier ZIP
    zip_close(z);

    printf("Unzip successful.\n");

    return 0;
}
