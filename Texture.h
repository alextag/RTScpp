#include <IL/IL.h>

class Texture
{
public:
    Texture();
    ~Texture();
    GLuint width;
    GLuint height;
    bool loadfile( std::string );
    bool loadPixels( GLuint* pixels, GLuint width, GLuint height );
    void freeTexture();
    GLuint id;
};

Texture::Texture()
{
    this->height = 0;
    this->width = 0;
    this->id = 0;
}

Texture::~Texture()
{
    this->freeTexture();
}

bool Texture::loadfile( std::string path )
{
    bool successload = false;
    ILuint imgID = 0;
    ilGenImages( 1, &imgID );
    ilBindImage( imgID );

    ILboolean success = ilLoadImage( path.c_str() );
    if (success == IL_TRUE)
    {
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if ( success == IL_TRUE )
        {
            successload = this->loadPixels( (GLuint*)ilGetData(), (GLuint)ilGetInteger( IL_IMAGE_WIDTH ), (GLuint)ilGetInteger( IL_IMAGE_HEIGHT ) );
        }
        ilDeleteImages( 1, &imgID );
    }
    if ( !successload )
    {
        printf( "Unable to load %s\n", path.c_str() );
    }
    return successload;
}

bool Texture::loadPixels( GLuint* pixels, GLuint width, GLuint height )
{
    this->freeTexture();
    this->width = width;
    this->height = height;
    glGenTextures( 1, &this->id );
    glBindTexture( GL_TEXTURE_2D, this->id );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glBindTexture( GL_TEXTURE_2D, (GLuint) NULL );
    GLenum error = glGetError();
    if ( error != GL_NO_ERROR )
    {
        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }
    return true;
}

void Texture::freeTexture()
{
    if ( this->id != 0 )
    {
        glDeleteTextures( 1, &this->id );
        this->id = 0;
    }
    this->width = 0;
    this->height = 0;
}
