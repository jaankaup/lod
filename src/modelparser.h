#ifndef MODELPARSER_H
#define MODELPARSER_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <exception>
#include "global.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

struct Material {
    std::string material;
    unsigned int start = 0;
    unsigned int end = 0;
};

struct Verteksi_indekseilla {
    int v_indeksi;
    int t_indeksi;
    int n_indeksi;
};

class ModelParser
{
    public:
        ModelParser();
        ~ModelParser();

        bool parsiVerteksiRivi(std::string rivi, glm::vec3& verteksi);
        bool parsiTekstuuriKoordinaatti(std::string rivi, glm::vec2& tk);
        bool parsiNormaaliVektori(std::string rivi, glm::vec3& normaali);
        bool parsiMateriaali(std::string rivi, Material& materiaali);
        //bool parseFaces(std::string line, Verteksi_indekseilla& face);
        bool parsiFace(std::string rivi);
        //bool parseFaces(std::string line);
        void parse(const std::string& fileLocation);
        int findIndex(const Verteksi_indekseilla& vi);
        void* getIndexArray();
        std::vector<float> getData();
        unsigned int getIndexSize();
        unsigned int getDataSize();
        unsigned int getIndiceCount();

    private:
        std::vector<glm::vec3> verteksit_;
        std::vector<glm::vec2> tekstuuri_koordinaatit_;
        std::vector<glm::vec3> normaalivektorit_;
        std::vector<Material> materiaaliryhmat_;
        std::vector<Verteksi_indekseilla> verteksitIndekseilla_;
        std::vector<unsigned int> indices_;

       /// Parses @param str to T which must be a arithmetic type. If an error occurs, returns false.
       /// Else returns true. TODO: change this to something better.
       template<typename T> inline bool parseNumber(const std::string& str, T& t)
       {
       static_assert(std::is_arithmetic<T>::value, "Arithmetic type is required.");
       std::stringstream ss(str);
       ss >> t;
       if (ss.fail())
       {
           logError.log("Failed to parse '%' to %.", str, typeid(T).name());
           return false;
       }
       return true;
       }
};



#endif // MODELPARSER_H
