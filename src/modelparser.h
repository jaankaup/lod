#ifndef MODELPARSER_H
#define MODELPARSER_H
#include <functional>
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

        bool parsiVerteksiRivi(const std::string& rivi, glm::vec3& verteksi);
        bool parsiTekstuuriKoordinaatti(const std::string& rivi, glm::vec2& tk, float textureTiling);
        bool parsiNormaaliVektori(const std::string& rivi, glm::vec3& normaali);
        bool parsiMateriaali(const std::string& rivi);
        //bool parseFaces(std::string line, Verteksi_indekseilla& face);
        bool parsiFace(const std::string& rivi);
        bool parsiFace3(const std::string& rivi);
        //bool parseFaces(std::string line);
        void parse(const std::string& fileLocation, float textureTiling);
        int findIndex(const Verteksi_indekseilla& vi);
        void* getIndexArray();
        std::vector<float> getData();
        unsigned int getIndexSize();
        unsigned int getDataSize();
        unsigned int getIndiceCount();
        std::vector<Material> getMaterials() const;

    private:
        std::vector<glm::vec3> verteksit_;
        std::vector<glm::vec2> tekstuuri_koordinaatit_;
        std::vector<glm::vec3> normaalivektorit_;
        std::vector<Material> materiaaliryhmat_;
        std::vector<Verteksi_indekseilla> verteksitIndekseilla_;
        std::vector<unsigned int> indices_;
        std::vector<Material> materials_;
};



#endif // MODELPARSER_H
