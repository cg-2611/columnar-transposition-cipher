#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *get_key_letters(const char *key, int *count)
{
    int letter_count = 0;
    int i, j;

    // count the number of alphanumeric characters in the key
    for (i = 0; i < strlen(key); i++)
    {
        if (isalnum(key[i]))
        {
            letter_count++;
        }
    }

    // allocate memory to store all the alphanumeric characters in the key
    char *key_letters = malloc((letter_count + 1) * sizeof(*key_letters));

    if (key_letters == NULL)
    {
        fprintf(stderr, "Error allocating memory for key\n");
        return NULL;
    }


    // add each alphanumeric character form the key to key_letters
    j = 0;
    for (i = 0; i < strlen(key); i++)
    {
        if (isalnum(key[i]))
        {
            key_letters[j++] = key[i];
        }
    }

    key_letters[j] = '\0';

    *count = letter_count;
    return key_letters;
}

char *get_file_letters(const char *filename, int *count)
{
    FILE *file;

    if ((file = fopen(filename, "r")) == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    int letter_count = 0;
    char c;

    // count the number of alphanumeric characters in the file
    while ((c = fgetc(file)) != EOF)
    {
        if (isalnum(c))
        {
            letter_count++;
        }
    }

    // allocate memory to store all the alphanumeric characters in the file
    char *file_letters = malloc((letter_count + 1) * sizeof(*file_letters));

    if (file_letters == NULL)
    {
        fprintf(stderr, "Error allocating memory for file characters\n");
        fclose(file);
        return NULL;
    }

    rewind(file); // go back to the beginning of the file

    // add all alphanumeric characters from the file to the string
    int i = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (isalnum(c))
        {
            file_letters[i++] = c;
        }
    }

    file_letters[i] = '\0';

    fclose(file);

    *count = letter_count;
    return file_letters;
}

void pad_string(char **file_letters, int *letter_count, int pad_size)
{
    char file_letters_no_padding[*letter_count];

    strcpy(file_letters_no_padding, *file_letters);

    *letter_count += pad_size; // update the  number of letters to account for the padding
    *file_letters = realloc(*file_letters, (*letter_count + 1) * sizeof(**file_letters));

    int i;
    for (i = *letter_count - pad_size; i < *letter_count; i++)
    {
        (*file_letters)[i] = 'X';
    }

    (*file_letters)[i] = '\0';
}

void encrypt(const char *filename, const char *key, char **result)
{
    int letter_count_key;  // number of alphanumeric characters in the key
    int letter_count_file; // number of alphanumeric characters in the message

    char *key_letters = get_key_letters(key, &letter_count_key);

    if (key_letters == NULL)
    {
        *result = NULL;
        return;
    }

    char *file_letters = get_file_letters(filename, &letter_count_file);

    if (key_letters == NULL)
    {
        *result = NULL;

        free(key_letters);
        key_letters = NULL;

        return;
    }

    // calculate the number of rows and columns required to store the message and the key in a  2D array
    int rows = (int)((letter_count_file / strlen(key_letters)) + 1); // add one for the key
    int columns = (int)strlen(key_letters);

    // calculate the number of X characters that will be needed as padding
    int X_number = columns - (letter_count_file % columns);

    if (letter_count_file % columns != 0)
    {
        rows++; // if the message does not fit perfectly in the grid, increment the number of rows
        pad_string(&file_letters, &letter_count_file, X_number);
    }
    else
    {
        X_number = 0; // if the message does fit, then there will be no X characters needed for padding
    }

    char table[rows][columns];

    int i, j, k;
    int swap_made = 0;

    // add the key to the top row of the grid
    for (i = 0; i < columns; i++)
    {
        table[0][i] = key_letters[i];
    }

    // add all other letters to the grid
    k = 0;
    i = 1;
    while (k < letter_count_file)
    {
        for (j = 0; j < columns; j++)
        {
            table[i][j] = file_letters[k];
            k++;
        }

        i++;
    }

    // sort the key using a bubble sort and rearrange the columns
    for (k = 0; k < columns - 1; k++)
    {
        swap_made = 0;
        for (j = 0; j < columns - k - 1; j++)
        {
            if (table[0][j] >= table[0][j + 1])
            {
                for (i = 0; i < rows; i++)
                {
                    // swap the characters of the key and all the characters in each column
                    // of the characters being swapped
                    char temp = table[i][j];
                    table[i][j] = table[i][j + 1];
                    table[i][j + 1] = temp;

                    swap_made = 1;
                }
            }
        }

        // exit the bubble sort if no swaps are made
        if (swap_made == 0)
        {
            break;
        }
    }

    // re-populate the file_letters array with the new encrypted message
    k = 0;
    for (i = 1; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            file_letters[k++] = table[i][j];
        }
    }

    file_letters[k] = '\0'; // put the null terminator on the end of the string

    // allocate memory for the result
    // if the allocation is successful, copy the encrypted message to result
    *result = malloc((letter_count_file + 1) * sizeof(**result));

    if (*result != NULL)
    {
       strcpy(*result, file_letters);
    }

    free(key_letters);
    key_letters = NULL;

    free(file_letters);
    file_letters = NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "\nInvalid arguments passed\n");
        fprintf(stderr, "\nUsage:\n");
        fprintf(stderr, "\tcolumnar_transposition_cipher <text_file_name> <key_for_cipher>\n\n");
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    const char *key = argv[2];

    char *encrypted_text = NULL;
    encrypt(filename, key, &encrypted_text);

    if (encrypted_text != NULL)
    {
        printf("%s\n", encrypted_text);
    }

    free(encrypted_text);
    encrypted_text = NULL;

    return EXIT_SUCCESS;
}
