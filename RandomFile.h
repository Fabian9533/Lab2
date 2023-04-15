#include <iostream>
#include<fstream>
#include<cstdio>
#include <map>
using namespace std;

struct Record
{
    char nombre[30];
    char carrera[20];
    int ciclo;

    void setData() {
        cout << "Alumno:";
        cin >> nombre;
        cout << "Carrera: ";
        cin >> carrera;
        cout << "Ciclo: ";
        cin >> ciclo;
    }

    void showData() {
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
    }

    string getKey() {
        return string(nombre) + "-" + string(carrera);
    }
};

class RandomFile {
private:
    string fileName;
    string indexName;
    map<string, long> index;

public:
    RandomFile(string _fileName) {
        this->fileName = _fileName;
        this->indexName = _fileName + "_ind";
        readIndex();
    }

    ~RandomFile(){
        writeIndex();
    }

    /*
    * leer el indice desde disco
    */
    void readIndex()
    {
        ifstream indexFile(indexName);
        if (indexFile.is_open()) {
            while (!indexFile.eof()) {
                string key;
                long posFisica;
                indexFile >> key >> posFisica;
                index[key] = posFisica;
            }
        }
        indexFile.close();
    }

    /*
    * Regresa el indice al disco
    */
    void writeIndex(){
        ofstream indexFile(indexName);
        for (auto& pair : index) {
            indexFile << pair.first << " " << pair.second << endl;
        }
        indexFile.close();
    }

    /*
    * Escribe el registro al final del archivo de datos. Se actualiza el indice.
    */
    void write_record(Record record) {
        ofstream dataFile;
        dataFile.open(this->fileName, ios::app | ios::binary);
        long posFisica = dataFile.tellp();
        dataFile.write((char*)&record, sizeof(Record));
        this->index[record.getKey()] = posFisica;
        dataFile.close();
    }

    /*
    * Busca un registro que coincida con la key
    */
    Record* search(string key) {
        Record* result = nullptr;
        auto it = index.find(key);
        if (it != index.end()) {
            ifstream dataFile(fileName);
            dataFile.seekg(it->second);
            dataFile.read((char*)&result, sizeof(Record));
            dataFile.close();
        }
        return result;
    }

    /*
    * Muestra todos los registros de acuerdo como fueron insertados en el archivo de datos
    */
    void scanAll() {
        ifstream dataFile(fileName);
        while (!dataFile.eof()) {
            Record record;
            dataFile.read((char*)&record, sizeof(Record));
            if (dataFile.eof()) break;
            record.showData();
        }
        dataFile.close();
    }

    /*
    * Muestra todos los registros de acuerdo a como estan ordenados en el indice
    */
    void scanAllByIndex() {
        for (auto& pair : index) {
            ifstream dataFile(fileName);
            dataFile.seekg(pair.second);
            Record record;
            dataFile.read((char*)&record, sizeof(Record));
            record.showData();
            dataFile.close();
        }
    }
};