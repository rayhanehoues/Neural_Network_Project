#ifndef NEURONE_MANIPULATION_H
#define NEURONE_MANIPULATION_H

//Initialiser un neurone avec un nombre d'entrées donné
Neurone* initializer_neurone(int num_xi)
{
    // Allouer de la mémoire pour un nouveau neurone
    Neurone* neurone = (Neurone*)malloc(sizeof(Neurone));

    // Vérifier si l'allocation de mémoire a réussi
    if (neurone == NULL)
    {
        printf("Erreur : Impossible d'allouer de la mémoire pour le neurone.\n");
        exit(EXIT_FAILURE);
    }

    neurone->num_xi = num_xi; // Assigner le nombre d'entrées du neurone

    // Allouer de la mémoire pour le tableau d'entrées (xi)
    neurone->xi = (double*)malloc(num_xi * sizeof(double));

    // Vérifier si l'allocation de mémoire a réussi
    if (neurone->xi == NULL)
    {
        printf("Erreur : Impossible d'allouer de la mémoire pour les entrées du neurone.\n");
        free(neurone); // Libérer la mémoire du neurone déjà allouée
        exit(EXIT_FAILURE);
    }

    // Allouer de la mémoire pour le tableau de poids (weights)(wi)
    neurone->wi = (double*)malloc(num_xi * sizeof(double));

    // Vérifier si l'allocation de mémoire a réussi
    if (neurone->wi == NULL)
    {
        printf("Erreur : Impossible d'allouer de la mémoire pour les poids du neurone.\n");
        free(neurone->xi); // Libérer la mémoire des entrées déjà allouée
        free(neurone); // Libérer la mémoire du neurone déjà allouée
        exit(EXIT_FAILURE);
    }

    // Initialiser les poids (weights) de manière aléatoire centrée autour de zéro
    for (int i = 0; i < num_xi; i++)
    {
        neurone->wi[i] = ((double)rand() / RAND_MAX) * 2 - 1; // Entre -1 et 1
    }

    // Initialiser le biais (bias) de manière aléatoire centrée autour de zéro
    neurone->bias = ((double)rand() / RAND_MAX) * 2 - 1; // Entre -1 et 1

    return neurone; // Retourner le pointeur vers le neurone initialisé
}

//Initialiser une couche de neuron
Couche* initializer_couche(int num_neurones, int num_xi_par_neurone)
{
    Couche* couche = (Couche*)malloc(sizeof(Couche)); //Allouer de la mémoire pour une nouvelle couche

    // Vérifier si l'allocation de mémoire a réussi
    if (couche == NULL)
	{
        printf("Erreur : Impossible d'allouer de la mémoire pour la couche de neurones.\n");
        exit ;
    }
    couche->num_neurones = num_neurones;//Assigner le nombre de neurones dans la couche

    couche->neurones = (Neurone**)malloc(num_neurones * sizeof(Neurone*));//Allouer de la mémoire pour le tableau de pointeurs de neurones

    //Vérifier si l'allocation de mémoire a réussi
    if (couche->neurones == NULL)
	{
        printf ("Erreur : Impossible d'allouer de la mémoire pour les neurones de la couche.\n");
        exit ;
    }

    //Initialiser les neurones dans la couche
    for (int i = 0; i < num_neurones; i++)
    {
        couche->neurones[i] = initializer_neurone(num_xi_par_neurone);

        // Demander à l'utilisateur de choisir la fonction d'activation
        int choix_fonction;
        printf("Choisir votre fonction d'activation pour le neurone %d:\n", i+1);
        printf("1. ReLU\n");
        printf("2. Sigmoid\n");
        printf("3. Softmax\n");
        printf("Enter your choice: ");
        scanf("%d", &choix_fonction);

        // Assigner la fonction d'activation pour ce neurone
        switch (choix_fonction)
        {
            case 1:
                couche->neurones[i]->activation_function = 1;
                break;
            case 2:
                couche->neurones[i]->activation_function = 2;
                break;
            case 3:
                couche->neurones[i]->activation_function = 3;
                break;
            default:
                printf("Choix invalide.");
                return NULL;
        }
    }

    return couche;//Retourner le pointeur vers la couche initialisée
}


//Calculer la sortie d'une couche de neurones
int compter_couche_yi(Couche* couche, double xi_valeurs[])
{
    // Pour chaque neurone de la couche
    for (int i = 0; i < couche->num_neurones; i++)
    {
        Neurone *neurone = couche->neurones[i];

        // Attribuer les entrées au neurone
        for (int j = 0; j < neurone->num_xi; j++)
        {
            neurone->xi[j] = xi_valeurs[j]; // Assigner les valeurs d'entrée au neurone
        }

        // Calculer la somme pondérée des entrées
        double somme = neurone->bias; // Commencer avec le biais du neurone
        for (int j = 0; j < neurone->num_xi; j++)
        {
            somme += neurone->xi[j] * neurone->wi[j]; // Additionner les valeurs d'entrée pondérées
        }

        // Appliquer les fonctions d'activation en fonction du type de neurone
        switch (neurone->activation_function) {
            case 1:
                neurone->yi = relu(somme); // Appliquer la fonction d'activation ReLU
                break;
            case 2:
                neurone->yi = sigmoid(somme); // Appliquer la fonction d'activation Sigmoide
                break;
            case 3:
                // Vérifier que le nombre d'entrées est correct pour la fonction softmax
                if (neurone->num_xi == 1) {
                    // Appliquer la fonction d'activation Softmax
                    softmax(neurone->xi, neurone->num_xi, &neurone->yi);
                } else {
                    printf("Erreur : Le nombre d'entrées pour la fonction Softmax doit être égal à 1.\n");
                    return 1;
                }
                break;
            default:
                printf("Erreur : Type de fonction d'activation invalide pour le neurone %d.\n", i+1);
                return 1;
        }
    }
    return 0;
}

#endif
