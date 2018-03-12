// File hashDriver.cpp:
// Place your identifying information here CLEARLY.

#include <iostream>

using namespace std;

class TableEntry {
private:
    string key;
    int value;
public:
    TableEntry(string key, int value) {
        this->key = key;
        this->value = value;
    }

    string getKey() { return key; }
    int getValue() { return value; }

};


#define TABLE_SIZE 101 // Change as necessary
#define EMPTY_KEY ""
#define TOMBSTONE_KEY "tombstone"
class Hasher {
private:
    TableEntry** table;
    char type;
    char crp;
    double loadFactor;

    // Define any other necessary helper functions that are not part of the public interface:
    //PRECONDITION: a string is given of 8 capital letters
    //POSTCONDITION: a number is returned based on the letters
    int goodHash(string key){
        int hashcode = 0;
        int index = 0;
        for(int i = 0; i < key.length(); i++){
            hashcode += key[i]+0;
        }
        return hashcode % TABLE_SIZE;
    }
    //PRECONDITION: a string is given of 8 capital letters
    //POSTCONDITION: a better number is returned based on the letters
    int poorHash(string key){
        int hashcode = 0;
        int index = 0;
        int r = 0;
        for(int i = 0; i <key.length(); i++){
            hashcode += key[i]+0 + r;
            r+= 2;
        }
        return hashcode % TABLE_SIZE;
    }

public:
    // Define the necessary constructors (be sure to initialize your table entries to NULL):

    // See assignment description.
    Hasher(char type, char crp){
        this->type = type;
        this->crp = crp;
        /*for(int i = 0; i < TABLE_SIZE; i++){
            table[i] = EMPTY;
        }*/
        //TableEntry* t = (TableEntry*) operator new (sizeof(TableEntry)*TABLE_SIZE);
        //table = &t;
        table = new TableEntry*[TABLE_SIZE];
        for(int i = 0; i < TABLE_SIZE; i++){
            table[i] = new TableEntry(EMPTY_KEY, 0);
        }
        //this->table = new tableentry[1][tablesize];
    }

    // See assignment description.
    Hasher(char type, char crp, double loadFactor, char* filename){
        this->type = type;
        this->crp = crp;
        this->loadFactor = loadFactor;
        /*for(int i = 0; i < TABLE_SIZE; i++){
            table[i] = EMPTY;
        }*/
        //TableEntry* t = (TableEntry*) operator new (sizeof(TableEntry)*TABLE_SIZE);
        //table = &t;
        /*for(int i = 0; i < TABLE_SIZE; i++){
            *table[i] = new TableEntry("empty", EMPTY);
        }*/
        //this->table = new tableentry[1][tablesize];
        table = new TableEntry*[TABLE_SIZE];
        for(int i = 0; i<TABLE_SIZE; i++){
            table[i] = new TableEntry(EMPTY_KEY, 3);
        }
    }
    //POSTCONDITION: this type is returned
    char getType(){ return this->type; }

    //POSTCONDITION: this crp is returned
    char getCrp(){ return this->crp; }
    //PRECONDITION: hash is index into hashtable from good or bad hash, quadIndex starts at 0,
    //              key is what to be found, sub is not an index into the array.
    //POSTCONDITION: table has been probed quadratically until object was found and the index returned, or
    //               key was not found.
    bool quadraticProbeSearch(int hash, int quadIndex, string key, int numProbes, int& sub){
        numProbes++;
        if(table[hash]->getKey() == EMPTY_KEY){
            return false;
        }

        if(table[hash]->getKey() == key){
            sub = hash;
            //cout << "number of probes = "<< numProbes << endl;
            return true;
        }
        quadIndex++;
        hash += quadIndex*quadIndex;
        hash = hash % TABLE_SIZE;
        return quadraticProbeSearch(hash, quadIndex, key, numProbes, sub);

    }

    //PRECONDITION: hash is index into hashtable from good or bad hash, quadIndex starts at 0,
    //              key is what to be found, sub is not an index into the array.
    //POSTCONDITION: table has been probed quadratically until object, or tombstone, or empty slot was
    //               found and the index returned so that the new entry can be inserted there, or key
    //               was not found.
    bool quadraticProbeInsert(int hash, int quadIndex, string key, int numProbes, int& sub){
        if (quadIndex >= (TABLE_SIZE-1)){
            cout << "table full" << endl;
            return false;
        }
        if((table[hash]->getKey() == EMPTY_KEY)||
            (table[hash]->getKey() == TOMBSTONE_KEY)||
            (table[hash]->getKey() == key)){
            //cout << "number of probes = " << numProbes << endl;
            sub = hash;
            return true;
        }
        quadIndex++;
        hash += quadIndex*quadIndex;
        hash = hash % TABLE_SIZE;
        numProbes++;
        return quadraticProbeInsert(hash, quadIndex, key, numProbes, sub);

    }
    //PRECONDITION: hash is index into hashtable from goodHash, coefficient starts at 0,
    //              key is what to be found, sub is not an index into the array
    //POSTCONDITION: table has been doubly hashed until object was found and the index returned,
    //               or key was not found.
    bool poorDoubleProbeSearch(int hash, int coefficient, string key, int numProbes, int& sub){
        numProbes++;
        if(table[hash]->getKey() == EMPTY_KEY){
            return false;
        }
        if(table[hash]->getKey() == key){
            sub = hash;
            //cout << "number of probes = "<< numProbes << endl;
            return true;
        }
        hash += (coefficient*poorHash(key));
        hash = hash % TABLE_SIZE;
        coefficient++;
        return poorDoubleProbeSearch(hash, coefficient, key, numProbes, sub);
    }

    //PRECONDITION: hash is index into hashtable from goodHash, coefficient starts at 0,
    //              key is what to be found, sub is not an index into the array
    //POSTCONDITION: table has been doubly hashed until object, or tombstone, or empty slot was
    //               found and the index returned so that the new entry can be inserted there,
    //               or key was not found.
    bool poorDoubleProbeInsert(int hash, int coefficient, string key, int numProbes, int& sub){
        if (coefficient >= (TABLE_SIZE-1)){
            cout << "full" << endl;
            cout << "table full" << endl;
            return false;
        }
        if((table[hash]->getKey() == EMPTY_KEY)||
            (table[hash]->getKey() == TOMBSTONE_KEY)||
            (table[hash]->getKey() == key)){
            //cout << "number of probes = " << numProbes << endl;
            sub = hash;
            return true;
        }
        hash += (coefficient*(poorHash(key)));
        hash = hash % TABLE_SIZE;
        coefficient++;
        numProbes++;
        return poorDoubleProbeInsert(hash, coefficient, key, numProbes, sub);

    }

    //PRECONDITION: hash is index into hashtable from poorHash, coefficient starts at 0,
    //              key is what to be found, sub is not an index into the array
    //POSTCONDITION: table has been doubly hashed until object was found and the index returned,
    //               or key was not found.
    bool goodDoubleProbeSearch(int hash, int coefficient, string key, int numProbes, int& sub){
        numProbes++;
        if(table[hash]->getKey() == EMPTY_KEY){
            return false;
        }
        if(table[hash]->getKey() == key){
            sub = hash;
            //cout << "number of probes = "<< numProbes << endl;
            return true;
        }
        hash += (coefficient*goodHash(key));
        hash = hash % TABLE_SIZE;
        coefficient++;
        return goodDoubleProbeSearch(hash, coefficient, key, numProbes, sub);
    }

    //PRECONDITION: hash is index into hashtable from poorHash, coefficient starts at 0,
    //              key is what to be found, sub is not an index into the array
    //POSTCONDITION: table has been doubly hashed until object, or tombstone, or empty slot was
    //               found and the index returned so that the new entry can be inserted there,
    //               or key was not found.
    bool goodDoubleProbeInsert(int hash, int coefficient, string key, int numProbes, int& sub){
        if (coefficient >= (TABLE_SIZE-1)){
            cout << "full" << endl;
            cout << "table full" << endl;
            return false;
        }
        if((table[hash]->getKey() == EMPTY_KEY)||
            (table[hash]->getKey() == TOMBSTONE_KEY)||
            (table[hash]->getKey() == key)){
            //cout << "number of probes = " << numProbes << endl;
            sub = hash;
            return true;
        }
        hash += (coefficient*(goodHash(key)));
        hash = hash % TABLE_SIZE;
        coefficient++;
        numProbes++;
        return goodDoubleProbeInsert(hash, coefficient, key, numProbes, sub);

    }

    //PRECONDITION: key is eight capital letters, subscript is not an index into array inent is either 's' or 'i'
    //POSTCONDITION: either(a) an empty space is found for inserting, (b) the key is found, or (c) nothing is found
    bool search(string key, int& subscript, char intent){
        char the_type = getType();
        char the_crp = getCrp();
        if(intent == 's'){
            if (the_type == 'g'){
                int hashcode = goodHash(key);
                if (the_crp == 'q'){
                    return quadraticProbeSearch(hashcode, 0, key, 0, subscript);
                }
                if (the_crp == 'd'){
                    return poorDoubleProbeSearch(hashcode, 0, key, 0, subscript);
                }
                else { cout << "invalid crp" << endl; return false;}
            }
            if (the_type == 'b'){
                int hashcode = poorHash(key);
                if (the_crp == 'q'){
                    return quadraticProbeSearch(hashcode, 0, key, 0, subscript);
                }
                if (the_crp == 'd'){
                    return goodDoubleProbeInsert(hashcode, 0, key, 0, subscript);
                }
                else { cout << "invalid crp" << endl; return false;}
            }
            else {cout << "invalid type" << endl; return false;}
        }
        if(intent == 'i'){
            if (the_type == 'g'){
                int hashcode = goodHash(key);
                if (the_crp == 'q'){
                    if(isFull()){
                        cout << "table full" << endl;
                        return false;
                    }
                    return quadraticProbeInsert(hashcode,0,key,0,subscript);
                }
                if (the_crp == 'd'){
                    if(isFull()){
                        cout << "table full" << endl;
                        return false;
                    }
                    return poorDoubleProbeInsert(hashcode, 0, key, 0, subscript);
                }
                else { cout << "invalid crp" << endl; return false;}
            }
            if (the_type == 'b'){
                int hashcode = poorHash(key);
                if (the_crp == 'q'){
                    if(isFull()){
                      cout << "table full" << endl;
                      return false;
                    }
                    else return quadraticProbeInsert(hashcode,0,key,0,subscript);
                }
                if (the_crp == 'd'){
                 if(isFull()){
                        cout << "table full" << endl;
                        return false;
                 }
                 return goodDoubleProbeInsert(hashcode, 0, key, 0, subscript);
                }
                else { cout << "invalid crp" << endl; return false;}
            }
         else {cout << "invalid type" << endl; return false;}
        }
        return false;
    }
    //PRECONDITION: key is valid.
    //POSTCONDITION: new entry is inserted in array or not b/c array is full.
    bool insert(string key, int value){
        int subscript = -1;
        if(search (key, subscript, 'i') == true){
            delete table[subscript];
            table[subscript] = new TableEntry(key,value);
            return true;
        }
        else { cout << "table full" << endl; return false;}
    }
    //PRECONDITION: key is valid.
    //POSTCONDITION: key is found and replaced with tombstone or not b/c it doesn't exist
    bool remove(string key){
        int subscript = -1;
        if(search(key, subscript, 's')== true){
            delete table[subscript];
            table[subscript] = new TableEntry(TOMBSTONE_KEY, -1);
            return true;
        }
        else { cout << "key not in table" << endl; return false;}
    }
    //POSTCONDITION: returned true if table is full or false if not full
    bool isFull(){
        int x = 0;
        for(int i = 0; i < TABLE_SIZE; i++){
            if(table[i]->getKey() == EMPTY_KEY){
                x--;
            }
            if(table[i]->getKey() != EMPTY_KEY){
                x++;
            }
        }
        return (x==TABLE_SIZE);
    }

    // Simply outputs the current contents of the table and the indices (you can write a loop
    // that just prints out the underlying array):
    // E.g.
    //    table->printTable() might generate:
    //        25  HBZEJKGA   1
    //        32  RHJMIVTA   2
    //
    void printTable(){
        for(int i = 0; i < TABLE_SIZE; i++){
            if(table[i]->getKey() != EMPTY_KEY){
                cout << i  << ' ' << table[i]->getKey() << ' ' << table[i]->getValue() << endl;
        }
    }
}

    // Define any other necessary functions that are part of the public interface:

    // Destructor-- do not alter.
    ~Hasher()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != NULL)
                delete table[i];
        delete[] table;
    }

};





// **Sample** main function/driver-- THIS IS NOT A COMPLETE TEST SUITE
// YOU MUST WRITE YOUR OWN TESTS
// See assignment description.
int main( int argc, char* argv[])
{
    // Generate empty hash tables:
    Hasher* goodHashRP1 = new Hasher('g', 'd');
    Hasher* goodHashQP1 = new Hasher('g', 'q');
    Hasher* badHashRP1 = new Hasher('b', 'd');
    Hasher* badHashQP1 = new Hasher('b', 'q');

    // Generate hash tables that are systematically loaded from file.
    // Note that if you cannot fit an element you should stop inserting elements
    // and set a flag to full.
    //Hasher* goodHashRP2 = new Hasher('g', 'd', 0.25, "filename.txt");
    //Hasher* goodHashQP2 = new Hasher('g', 'q', 0.5, "filename.txt");
    //Hasher* poorHashRP2 = new Hasher('b', 'd', 0.75, "filename.txt");
    //Hasher* poorHashQP2 = new Hasher('b', 'q', 0.25, "filename.txt");

    // Sample use case:
    int value = 10;
    if(goodHashRP1->insert("HBZEJKGA", value))
        cout << "Inserted" << endl;
    else
        cout << "Failed to insert" << endl;

    int subscript = -1;

    if(goodHashRP1->search("HBZEJKGA", subscript, 's'))
        cout << "Found at " << subscript << endl;
    else
        cout << "Failed to insert" << endl;
    goodHashRP1->printTable();

    if(goodHashRP1->remove("HBZEJKGA"))
        cout << "Removed" << endl;
    else
        cout << "Not deleted/not found" << endl;
    ////////////////////////////////////////////////////////////////////////////////////
    if(goodHashQP1->insert("HBZEJKGA", 2))
        cout << "Inserted" << endl;
    else
        cout << "Failed to insert" << endl;

    subscript = -1;

    if(goodHashQP1->search("HBZEJKGA", subscript, 's'))
        cout << "Found at " << subscript << endl;
    else
        cout << "Failed to insert" << endl;
    goodHashQP1->printTable();

    if(goodHashQP1->remove("HBZEJKGA"))
        cout << "Removed" << endl;
    else
        cout << "Not deleted/not found" << endl;
    //////////////////////////////////////////////////////////////////////////////////////////
    if(badHashRP1->insert("HBZEJKGA", 1))
        cout << "Inserted" << endl;
    else
        cout << "Failed to insert" << endl;

    subscript = -1;

    if(badHashRP1->search("HBZEJKGA", subscript, 's'))
        cout << "Found at " << subscript << endl;
    else
        cout << "Failed to insert" << endl;
    badHashRP1->printTable();

    if(badHashRP1->remove("HBZEJKGA"))
        cout << "Removed" << endl;
    else
        cout << "Not deleted/not found" << endl;
    ///////////////////////////////////////////////////////////////////////////////////////
    if(badHashQP1->insert("HBZEJKGA", 3))
        cout << "Inserted" << endl;
    else
        cout << "Failed to insert" << endl;

    subscript = -1;

    if(badHashQP1->search("HBZEJKGA", subscript, 's'))
        cout << "Found at " << subscript << endl;
    else
        cout << "Failed to insert" << endl;
    badHashQP1->printTable();
    if(badHashQP1->remove("HBZEJKGA"))
        cout << "Removed" << endl;
    else
        cout << "Not deleted/not found" << endl;
    return 0;
}
