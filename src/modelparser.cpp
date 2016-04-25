#include "modelparser.h"
#include "misc.h"

ModelParser::ModelParser()
{
    verteksit_.reserve(5000);
    normaalivektorit_.reserve(5000);
    tekstuuri_koordinaatit_.reserve(5000);
    materiaaliryhmat_.reserve(5000);
    verteksitIndekseilla_.reserve(5000);
    indices_.reserve(5000);
}

ModelParser::~ModelParser()
{
    //dtor
}

void ModelParser::parse(const std::string& fileLocation)
{
    std::string source = Misc::loadSource(fileLocation);
    std::stringstream ss(source);
    std::string line;

    while (std::getline(ss,line,'\n'))
    {
        glm::vec3 v;//parsitaan verteksit
        if (parsiVerteksiRivi(line,v)) {
            verteksit_.push_back(v);
            continue;
        }

        glm::vec2 vt;//parsitaan tekstuurikoordinaatit
        if (parsiTekstuuriKoordinaatti(line,vt)) {
            tekstuuri_koordinaatit_.push_back(vt);
            continue;
        }

        glm::vec3 vn;//parsitaan normaalivektorit
        if (parsiNormaaliVektori(line,vn)){
            normaalivektorit_.push_back(vn);
            continue;
        }

        // if (parsiMateriaali(line)

        if (!parsiFace(line)) continue;
    }

}

bool ModelParser::parsiVerteksiRivi(std::string rivi, glm::vec3& verteksi){
    static const std::regex verterex("v\\s+(\\-?\\d+\\.\\d+)\\s+(\\-?\\d+\\.\\d+)\\s+(\\-?\\d+\\.\\d+)");

    std::smatch tulos;
    if (!regex_match(rivi, tulos, verterex)) return false;

    std::string apu1 = tulos[1];
    std::string apu2 = tulos[2];
    std::string apu3 = tulos[3];

    glm::vec3 v((float)atof(apu1.c_str()), (float)atof(apu2.c_str()), (float)atof(apu3.c_str()));
    verteksi = v;

    return true;
}

bool ModelParser::parsiTekstuuriKoordinaatti(std::string rivi, glm::vec2& tk) {
    static const std::regex texrex("vt\\s+(\\-?\\d+\\.\\d+)\\s+(\\-?\\d+\\.\\d+)");

    std::smatch tulos;
    if (!regex_match(rivi, tulos, texrex)) return false;

    std::string apu1 = tulos[1];
    std::string apu2 = tulos[2];

    glm::vec2 v((float)atof(apu1.c_str()), (float)atof(apu2.c_str()));
    tk = v;
    return true;
}

bool ModelParser::parsiNormaaliVektori(std::string rivi, glm::vec3& normaali){
    static const std::regex norrex("vn\\s+(\\-?\\d+\\.\\d+)\\s+(\\-?\\d+\\.\\d+)\\s+(\\-?\\d+\\.\\d+)");

    std::smatch tulos;
    if (!regex_match(rivi, tulos, norrex)) return false;

    std::string apu1 = tulos[1];
    std::string apu2 = tulos[2];
    std::string apu3 = tulos[3];

    glm::vec3 v((float)atof(apu1.c_str()), (float)atof(apu2.c_str()), (float)atof(apu3.c_str()));
    normaali = v;

    return true;
}

bool ModelParser::parsiMateriaali(std::string rivi, Material& materiaali){
    static const std::regex marex("usemtl\\s+(\\S+)");

    std::smatch tulos;
    if (!regex_match(rivi, tulos, marex)) return false;

    return true;
}

bool ModelParser::parsiFace(std::string rivi){
    using std::string;
	static const std::regex farex4("f\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\/(\\d+)");
    static const std::regex farex3("f\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\/(\\d+)\\s+(\\d+)\\/(\\d+)\\/(\\d+)");

    std::smatch tulos;
    bool nelja = false;
    if (regex_match(rivi, tulos, farex4))
    {
        nelja = true;
    }
    else if (!regex_match(rivi, tulos, farex3))
    {
        return false;
    }

    string apu1 = tulos[1];
    string apu2 = tulos[2];
    string apu3 = tulos[3];
    string apu4 = tulos[4];
    string apu5 = tulos[5];
    string apu6 = tulos[6];
    string apu7 = tulos[7];
    string apu8 = tulos[8];
    string apu9 = tulos[9];
    string apu10 = tulos[10];
    string apu11 = tulos[11];
    string apu12 = tulos[12];

    Verteksi_indekseilla v1;
    v1.v_indeksi = (int)atoi(apu1.c_str());
    v1.t_indeksi = (int)atoi(apu2.c_str());
    v1.n_indeksi = (int)atoi(apu3.c_str());

    Verteksi_indekseilla v2;
    v2.v_indeksi = (int)atoi(apu4.c_str());
    v2.t_indeksi = (int)atoi(apu5.c_str());
    v2.n_indeksi = (int)atoi(apu6.c_str());

    Verteksi_indekseilla v3;
    v3.v_indeksi = (int)atoi(apu7.c_str());
    v3.t_indeksi = (int)atoi(apu8.c_str());
    v3.n_indeksi = (int)atoi(apu9.c_str());

    Verteksi_indekseilla v4;

    if (nelja)
    {
        v4.v_indeksi = (int)atoi(apu10.c_str());
        v4.t_indeksi = (int)atoi(apu11.c_str());
        v4.n_indeksi = (int)atoi(apu12.c_str());
    }

    // TODO: copypaste meininki. korjaa!
    /* V1 */
    int v1Indeksi = findIndex(v1);
    if (v1Indeksi == -1)
    {
        v1Indeksi = verteksitIndekseilla_.size();
        verteksitIndekseilla_.push_back(v1);
        indices_.push_back(v1Indeksi);
    }
    else indices_.push_back(v1Indeksi);

    /* V2 */
    int v2Indeksi = findIndex(v2);
    if (v2Indeksi == -1)
    {
        v2Indeksi = verteksitIndekseilla_.size();
        verteksitIndekseilla_.push_back(v2);
        indices_.push_back(v2Indeksi);
    }
    else indices_.push_back(v2Indeksi);

    /* V3 */
    int v3Indeksi = findIndex(v3);
    if (v3Indeksi == -1)
    {
        v3Indeksi = verteksitIndekseilla_.size();
        verteksitIndekseilla_.push_back(v3);
        indices_.push_back(v3Indeksi);
    }
    else indices_.push_back(v3Indeksi);

    if (!nelja) return true;

    indices_.push_back(v1Indeksi);
    indices_.push_back(v3Indeksi);

    /* V4 */
    int v4Indeksi = findIndex(v4);
    if (v4Indeksi == -1)
    {
        v4Indeksi = verteksitIndekseilla_.size();
        verteksitIndekseilla_.push_back(v4);
        indices_.push_back(v4Indeksi);
    }
    else indices_.push_back(v4Indeksi);

    return true;
}

int ModelParser::findIndex(const Verteksi_indekseilla& vi)
{
    for (unsigned int i=0 ; i<verteksitIndekseilla_.size() ; ++i)
    {
        if (verteksitIndekseilla_[i].v_indeksi == vi.v_indeksi &&
            verteksitIndekseilla_[i].t_indeksi == vi.t_indeksi &&
            verteksitIndekseilla_[i].n_indeksi == vi.n_indeksi)
        {
            /* Indeksi löytyi */
            return i;
        }
    }
    /* Indeksiä ei löytynyt. */
    return -1;
}

void* ModelParser::getIndexArray()
{
    return &indices_[0]; // no mitäs jos on tyhjä!!!
}

std::vector<float> ModelParser::getData()
{
    std::vector<float> temp;
    temp.reserve(20000);
    for (const auto& obj : verteksitIndekseilla_)
    {
        auto indV = obj.v_indeksi-1;
        auto indT = obj.t_indeksi-1;
        auto indN = obj.n_indeksi-1;

        glm::vec3 v = verteksit_[indV];
        glm::vec2 t = tekstuuri_koordinaatit_[indT];
        glm::vec3 n = normaalivektorit_[indN];

        temp.push_back(v.x); temp.push_back(v.y); temp.push_back(/*(-1.0f)**/v.z);
        temp.push_back(t.x); temp.push_back(t.y);
        temp.push_back(n.x); temp.push_back(n.y); temp.push_back(/*(-1.0f)**/n.z);
    }
    return temp;
}

unsigned int ModelParser::getIndexSize()
{
    return (sizeof(unsigned int) * indices_.size());
}

unsigned int ModelParser::getDataSize()
{
    return (sizeof(Verteksi_indekseilla) * verteksitIndekseilla_.size());
}

unsigned int ModelParser::getIndiceCount()
{
    return indices_.size();
}
