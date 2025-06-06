
#include "ModelFactory.h"

#include <stack>
#include <iostream>

namespace {

Texture::Type convertTextureType(aiTextureType type) {
    switch(type) {
        case aiTextureType_DIFFUSE: return Texture::Type::Diffuse;
        case aiTextureType_SPECULAR: return Texture::Type::Specular;
        default: throw std::runtime_error("We don't currently support that texture type");
    }
}

void loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::filesystem::path& directory, const std::vector<Texture>& textures, std::vector<unsigned int> &textureIndices) {
    unsigned int count = mat->GetTextureCount(type);
    for(unsigned int i = 0;i < count;i++) {
        aiString aiPath;
        mat->GetTexture(type, i, &aiPath);
        std::filesystem::path path = aiPath.C_Str();
        std::filesystem::path fullPath = directory / path;
        auto it = std::ranges::find_if(textures, [&fullPath](const Texture& tex) { return tex.getPath() == fullPath; });
        if(it != textures.end()) {
            textureIndices.push_back(it - textures.begin());
        } else {
            throw std::runtime_error("Texture should have been loaded but wasn't");
        }
    }
}

std::vector<Texture> loadTextures(const std::filesystem::path& directory, const aiScene* scene) {
    std::vector<Texture> textures;
    std::array<aiTextureType, 2> types = {aiTextureType_DIFFUSE, aiTextureType_SPECULAR};
    for(unsigned int i = 0;i < scene->mNumMaterials;i++) {
        aiMaterial* mat = scene->mMaterials[i];
        for(aiTextureType type : types) {
            unsigned int count = mat->GetTextureCount(type);
            for(unsigned int j = 0;j < count;j++) {
                aiString aiPath;
                mat->GetTexture(type, j, &aiPath);
                std::filesystem::path path = aiPath.C_Str();
                textures.push_back(Texture(directory / path, convertTextureType(type)));
            }
        }
    }
    return textures;
}

Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::filesystem::path& directory, const std::vector<Texture>& textures) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> textureIndices;

    vertices.reserve(mesh->mNumVertices);
    for(unsigned int i = 0;i < mesh->mNumVertices;i++) {
        vertices.push_back(Vertex(
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
            mesh->mTextureCoords[0] != nullptr 
                ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) 
                : glm::vec2(0.0f, 0.0f)
        ));
    }

    for(unsigned int i = 0;i < mesh->mNumFaces;i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0;j < face.mNumIndices;j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


    // Commenting this out because it is not needed right now but it was difficult to figure out
    // and it can be kept in case we ever want to have support for different illumination models.
    // ---------------------------------------
    // int illumModel;
    // if(material->Get("$mat.illum", 0, 0, illumModel) != AI_SUCCESS) {
    //     std::cerr << "Failed to find illuminaiton model";
    //     illumModel = 0;
    // }

    aiColor3D colorProp(0.0f, 0.0f, 0.0f);
    std::optional<Color> color = std::nullopt;
    if(material->Get(AI_MATKEY_COLOR_DIFFUSE, colorProp) == AI_SUCCESS) {
        color = Color(colorProp.r, colorProp.g, colorProp.b);
    }

    loadMaterialTextures(material, aiTextureType_DIFFUSE, directory, textures, textureIndices);
    loadMaterialTextures(material, aiTextureType_SPECULAR, directory, textures, textureIndices);
    
    return Mesh(
        std::move(color),
        std::move(vertices),
        std::move(indices),
        std::move(textureIndices));
}

std::vector<Mesh> processNode(aiNode* root, const aiScene* scene, const std::filesystem::path& directory, const std::vector<Texture>& textures) {
    std::vector<Mesh> meshes;
    std::stack<aiNode*> nodeStack;
    nodeStack.push(root);
    while(!nodeStack.empty()) {
        aiNode* node = nodeStack.top(); nodeStack.pop();
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene, directory, textures));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            nodeStack.push(node->mChildren[i]);
        }
    }
    return meshes;    
}

}
    
DrawableComponent ModelFactory::loadModel(const std::filesystem::path& path, LightingType lightingType) {
    std::filesystem::path directory = path.parent_path();
    std::cout << "Loading model from: " << path << '\n';
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::ostringstream ss;
        ss << "Failed to load scene " << importer.GetErrorString();
        throw std::runtime_error(ss.str());
    }

    std::vector<Texture> textures = loadTextures(directory, scene);
    std::vector<Mesh> meshes = processNode(scene->mRootNode, scene, directory, textures);
    auto model = std::unique_ptr<Model>(new Model(lightingType, std::move(directory), std::move(meshes), std::move(textures)));
    return DrawableComponent(std::move(model));
}

DrawableComponent CubeModelFactory::createCube(const Color &color, LightingType lightingType) { 
    const float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int stride = 8 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    auto model = std::unique_ptr<IDrawable>(new CubeModel(lightingType, VAO, VBO, color));
    return DrawableComponent(std::move(model));
}
