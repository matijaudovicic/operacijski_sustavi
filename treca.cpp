#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//Progam koji simulira tijek rezervacije stolova u nekom restoranu.Svaka dretva nakon jedne sekunde provjerava ima li slobodnih stolova te slucajno odabire jedan od njih.Nakon odaira dretva ulazi u kriticni odsjecak te ponovo provjerava jeli odabrani stol slonodan.Ako jest oznacava stol zauzetim i izlazi iz kriticnog odsjecka.Prilikom  ispisa za svaki stol mora biti vidljio jeli slobodan ili broj dretve koja je taj stol rezervirala.Svaka dretva ponavlja isti postupak sve dok vise nema slobodni stolova.Program zavrsava kada se sve dretve zavrse.Zaštitu kritičnog odsječka postupka rezervacije stola ostvariti koristeći Lamportov algoritam međusobnog isključivanja.

#define true 1
#define false 0

int n_threads; // Broj dretvi
int n_tables; // Broj stolova
int tables[100]; // Polje koji predstavlja stolove
int lamport_timestamp[100]; // Polje koji predstavlja Lamportov timestamp

void* reservation(void* arg) {
    int id = *((int*) arg);
    while (true) {
        sleep(1); // cekaj jednu sekundu
        int table = rand() % n_tables; // Slucajno odaberi stol
        int max_timestamp = 0;

        // Uði u kriticni odsjecak - provjeri stanje stola
        for (int i = 0; i < n_threads; i++) {
            if (lamport_timestamp[i] > max_timestamp) {
                max_timestamp = lamport_timestamp[i];
            }
        }
        lamport_timestamp[id] = max_timestamp + 1;
        if (tables[table] == -1) {
            tables[table] = id;
            printf("Dretva %d: rezerviram stol %d, stanje: ", id, table);
            for (int i = 0; i < n_tables; i++) {
                if (tables[i] == -1) {
                    printf("-");
                } else {
                    printf("%d", tables[i]);
                }
            }
            printf("\n");
            break;
        } else {
            printf("Dretva %d: neuspjela rezervacija stola %d, stanje: ", id, table);
            for (int i = 0; i < n_tables; i++) {
                if (tables[i] == -1) {
                    printf("-");
                } else {
                    printf("%d", tables[i]);
                }
            }
            printf("\n");
        }
    }
    return NULL;
}

int main() {
  

    n_threads = 5;//5 dretvi proizvoljne vrijednosti 
    n_tables =5;//5 stolova

    for (int i = 0; i < n_tables; i++) {
        tables[i] = -1; // Inicijaliziraj sve stolove kao slobodne
    }

    pthread_t threads[n_threads];
    int thread_ids[n_threads];

    for (int i = 0; i < n_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, reservation, &thread_ids[i]);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

