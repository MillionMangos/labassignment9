#include <stdio.h>
#include <stdlib.h>
//Note to Self: Size is 11
// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
	struct RecordType *next;
    // create a next pointer (for linkedlist structure)
};

// Fill out this structure
struct HashType
{
	struct RecordType *data;
    // create a pointer to a RecordType
};

// Compute the hash function
int hash(int x, int tableSize)
{
    // hash function is x % tableSize
	int key = x % tableSize;
	return key;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    // call the hash function to get the index
	int index = hash(record->id, tableSize);
    // if the RecordType at that index is NULL
	if(hashTable[index].data == NULL)
	{
		// set 'record' equal to the HashType pointer in the table at index
		hashTable[index].data = record;
	}
	// else
        // traverse to the end of the linkedlist and add 'record' to the end of it
	else
	{
		struct RecordType *newNode = (struct RecordType*)malloc(sizeof(struct RecordType));
		struct RecordType *last = hashTable[index].data;
		newNode = record;
		newNode->next = NULL;
		while(last->next != NULL)
		{
			last = last->next;
		}
		last->next = newNode;
	}
}
// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize)
{
    // for each entry in the table
        // print the contents in that index
        // The output should look like this (for example): "Index 1 -> 21715, Q, 16 -> 28876, a, 26 -> NULL"

	for(int i = 0; i < tableSize; i++)
	{
		if(hashTable[i].data != NULL)
		{
			printf("Index %d ", i);
		while(hashTable[i].data != NULL)
		{
			printf("-> %d, %c, %d", hashTable[i].data->id, hashTable[i].data->name, hashTable[i].data->order);
			hashTable[i].data = hashTable[i].data->next;
		}
		printf(" -> NULL \n");
		}
		
	}
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
	// Initialize the hash table
	struct HashType *HashTable;
    // create a variable hashTableSize and assign it a value
	int hashTableSize = recordSz;
    // initialize a hashTable, use calloc (so everything is assigned to NULL)
	HashTable = (struct HashType*)calloc(hashTableSize, sizeof(struct HashType) * hashTableSize);
    // for each record in pRecords, insert it into the hash table using the insertRecord function
	for(int i = 0; i < hashTableSize; i++)
	{
		insertRecord(HashTable, pRecords, hashTableSize);
		pRecords++;
	}
    // call the display records function
	displayRecordsInHash(HashTable,hashTableSize);
    // free all the allocated memory
	for(int i = 0; i < hashTableSize; i++)
	{
		if(HashTable[i].data != NULL)
		{
			//TO-DO, make sure this checks that there is not anything after it
			if(HashTable[i].data->next == NULL)
			{
					free(HashTable[i].data);
			}
		}	
		else
		{
			struct RecordType *temp;
			int count;
			while(HashTable[i].data->next != NULL)
			{
				HashTable[i].data = HashTable[i].data->next;
				count++;
			}
			for(int i = 0; i < count; i++)
			{
				while(HashTable[i].data->next != NULL)
				{
					HashTable[i].data = HashTable[i].data->next;
				}
				free(HashTable[i].data);
			}
			free(HashTable[i].data);
		}
	}
	free(HashTable);
    free(pRecords);

    return 0;
}