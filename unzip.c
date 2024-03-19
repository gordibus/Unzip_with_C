#include <stdio.h>
#include <stdlib.h>
#include <zip.h>

int main() {
    char zip_path[256];
    printf("Veuillez entrer le chemin du fichier ZIP à décompresser : ");
    scanf("%255s", zip_path); // Limiter la taille de l'entrée à 255 caractères pour éviter un dépassement de tampon

    // Ouvrir le fichier ZIP
    int err = 0;
    zip_t *z = zip_open(zip_path, 0, &err);

    if (z == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier ZIP.\n");
        return 1;
    }

    // Obtenir le nombre d'entrées dans le fichier ZIP
    int num_entries = zip_get_num_entries(z, 0);
    if (num_entries < 0) {
        fprintf(stderr, "Erreur lors de la récupération du nombre d'entrées dans le fichier ZIP.\n");
        zip_close(z);
        return 1;
    }

    // Parcourir chaque entrée dans le fichier ZIP
    for (int i = 0; i < num_entries; i++) {
        // Obtenir les informations sur l'entrée
        struct zip_stat st;
        zip_stat_index(z, i, 0, &st);

        // Ouvrir l'entrée
        zip_file_t *f = zip_fopen_index(z, i, 0);
        if (f == NULL) {
            fprintf(stderr, "Erreur lors de l'ouverture de l'entrée %d dans le fichier ZIP.\n", i);
            zip_close(z);
            return 1;
        }

        // Créer un fichier avec le nom de l'entrée
        FILE *output_file = fopen(st.name, "wb");
        if (output_file == NULL) {
            fprintf(stderr, "Erreur lors de la création du fichier de sortie pour l'entrée %d.\n", i);
            zip_fclose(f);
            zip_close(z);
            return 1;
        }

        // Lire le contenu de l'entrée et écrire dans le fichier de sortie
        char buf[1024];
        zip_int64_t nbytes;
        while ((nbytes = zip_fread(f, buf, sizeof(buf))) > 0) {
            fwrite(buf, 1, nbytes, output_file);
        }

        // Fermer les fichiers
        fclose(output_file);
        zip_fclose(f);
    }

    // Fermer le fichier ZIP
    zip_close(z);

    printf("Décompression terminée avec succès.\n");

    return 0;
}
