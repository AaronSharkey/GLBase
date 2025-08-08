/**
 * @file texture.h
 * @brief OpenGL texture loading and management with configurable parameters.
 *
 * @author Aaron Sharkey
 * @date 2025-07-30
 */

#pragma once

#include <iostream>
#include <stb_image.h>

#include "glad/glad.h"

/**
 * @class Texture
 * @brief OpenGL texture with configurable loading and filtering options.
 */
class Texture {
public:
    /**
     * @struct TextureConfig
     * @brief Configuration options for texture loading and filtering.
     */
    struct TextureConfig {
        GLenum wrapS = GL_REPEAT; ///< Texture wrapping mode for S coordinate
        GLenum wrapT = GL_REPEAT; ///< Texture wrapping mode for T coordinate
        GLenum minFilter = GL_LINEAR; ///< Texture minification filter
        GLenum magFilter = GL_LINEAR; ///< Texture magnification filter
        bool generateMipmap = true; ///< Whether to generate mipmaps
        bool flipVertically = false; ///< Whether to flip texture vertically on load
    };

    /**
     * @brief Load texture from file with default configuration.
     * @param texturePath Path to texture image file
     */
    explicit Texture(const char *texturePath)
        : Texture(texturePath, TextureConfig{}) {
    }

    /**
     * @brief Load texture from file with custom configuration.
     * @param texturePath Path to texture image file
     * @param config Texture loading and filtering options
     */
    explicit Texture(const char *texturePath, const TextureConfig &config);

    /**
     * @brief Bind texture to specified texture unit.
     * @param unit Texture unit to bind to (0-based)
     */
    void bind(int unit) const;

    /**
     * @brief Clean up OpenGL texture resources.
     */
    ~Texture();

private:
    int width, height, nrChannels; ///< Texture dimensions and number of channels
    const char *texturePath; ///< Path to the texture file
    unsigned int textureID; ///< OpenGL texture ID
    unsigned char *data; ///< Pointer to texture data loaded from file
};

inline Texture::Texture(const char *texturePath, const TextureConfig &config) : texturePath(texturePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);

    if (config.flipVertically) {
        stbi_set_flip_vertically_on_load(true);
    }

    data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (data) {
        const GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (config.generateMipmap) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

inline void Texture::bind(const int unit) const {
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

inline Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}
