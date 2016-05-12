#include "model.h"

Model::~Model()
{
    //dtor
}
/*
Model::Model(Model&& other)
{
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;
    material_ = other.material_;
}

Model& Model::operator=(Model&& other)
{
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;
    return *this;
}
*/
void Model::create(const std::string& fileLocation)
{
    ModelParser mp;
    mp.parse(fileLocation);
    std::vector<float> dat = mp.getData();

    vao_.bind();
    vbo_.setData(&dat[0], sizeof(float)*dat.size(), AttributeConfiguration::VTN_Interleaved);
    ibo_.setData(mp.getIndexArray(), mp.getIndexSize(), mp.getIndiceCount());
    material_ = mp.getMaterials();

    vao_.unBind();
    /*
    vao_.bind();

    std::vector<float> dat = mp.getData();
    vbo_.setData(&dat[0], sizeof(float)*dat.size(), AttributeConfiguration::VTN_Interleaved);
    ibo_.setData(mp.getIndexArray(), mp.getIndexSize(), mp.getIndiceCount());
    material_ = mp.getMaterials();

    vao_.unBind();
    */
}

VAO& Model::getVAO()
{
    //return vao_.get();
    return vao_.getInstance();
}

VertexBuffer& Model::getVertexBuffer()
{
    return vbo_.getInstance();
    //return vbo_;
}

IndexBuffer& Model::getIndexBuffer()
{
    return ibo_.getInstance();
    //return ibo_;
}

std::vector<Command> Model::createCommands() const
{
    std::vector<Command> commands;

    for (const auto& mat : material_)
    {
        Command cb;
        MaterialInfo temp = MaterialManager::getInstance().getMaterialInfo(mat.material);
        cb.vao = vao_.vaoID_;
        cb.shader = temp.shaderName;
        cb.texture = temp.textureName;
        cb.startIndex = mat.start;
        cb.indexCount = mat.end - mat.start;
        cb.model = model_;
        logDebug.log("cb.vao = %, cb.shader = %, cb.texture= %, cb.startIndex = %, "
                     "cb.indexCount = %", cb.vao,cb.shader,cb.texture,cb.startIndex,cb.indexCount);
        commands.push_back(cb);
    }
    return commands;
}

void Model::setModelMatrix(const glm::mat4& matrix)
{
    model_ = matrix;
}
/*
glm::mat4 Model::getModelMatrix() const
{
    return model_;
}
*/
