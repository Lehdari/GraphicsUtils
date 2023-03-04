//
// Project: DooT2
// File: Texture.inl
//
// Copyright (c) 2023 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

template <typename T_Data>
void Texture::updateFromBuffer(const T_Data* buffer, GLenum format, int width, int height)
{
    if (width >= 0)
        _width = width;
    if (height >= 0)
        _height = height;

    _dataType = typeToGLEnum<T_Data>();

    glBindTexture(_target, _textureIds[_activeId]);
    glTexImage2D(_target, 0, _channelFormat, _width, _height, 0, format, _dataType, buffer);
    glGenerateMipmap(_target);
    glBindTexture(_target, 0);
}
