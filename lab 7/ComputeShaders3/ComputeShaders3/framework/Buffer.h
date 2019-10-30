#pragma once

#include "glfuncs.h"
#include <vector>

class Buffer{
    
  private:
  
    Buffer( int size, GLenum usage, GLenum bufferType, bool mappable){
        init( size, usage, bufferType, mappable );
    }
    
  public:
  
    GLuint buffID;
    unsigned size;
    GLenum usage;
    void* ptr;  //only for mappable
    
    static std::shared_ptr<Buffer> create(unsigned size, GLenum usage = GL_STATIC_DRAW, GLenum bufferType = GL_ARRAY_BUFFER){
        Buffer* p = new Buffer(size, usage, bufferType, false);
        return std::shared_ptr<Buffer>(p);
    }
    
    template<typename T>
    static std::shared_ptr<Buffer> create(const std::vector<T>& initialData, GLenum usage = GL_STATIC_DRAW, GLenum bufferType = GL_ARRAY_BUFFER){
        Buffer* p = new Buffer(initialData.size()*sizeof(T), usage, bufferType, false);
        p->setData(initialData, bufferType);
        return std::shared_ptr<Buffer>(p);
    }

    static std::shared_ptr<Buffer> createMappable(unsigned size){
        Buffer* p = new Buffer(size, 0, 0, true);
        return std::shared_ptr<Buffer>(p);
    }
    
    template<typename T>
    static std::shared_ptr<Buffer> createMappable(const std::vector<T>& initialData, GLenum usage = GL_STATIC_DRAW, GLenum bufferType = GL_ARRAY_BUFFER){
        Buffer* p = new Buffer(initialData.size()*sizeof(T), usage, bufferType, true);
        p->setData(initialData, bufferType);
        return std::shared_ptr<Buffer>(p);
    }
    
    ~Buffer(){
        if( ptr ){
            bind(GL_ARRAY_BUFFER);
            glUnmapBuffer(GL_ARRAY_BUFFER);
            ptr=nullptr;
        }
        glDeleteBuffers(1, &buffID);
        buffID=0;
    }
    
    void bind(GLenum bindingPoint){
        glBindBuffer(bindingPoint, buffID);
    }

    void bindBase(GLenum bindingPoint, int index){
        glBindBufferBase(bindingPoint,index,buffID);
    }
    
    template<typename T>
    void setData(const std::vector<T>& newData, GLenum bufferType){
        setData(newData.data(), newData.size()*sizeof(T), bufferType);
    }
    
    void setData(const void* p, size_t size, GLenum bufferType){
        glBindBuffer(bufferType, buffID);
        if( size == this->size ){
            if( ptr )
                memcpy(ptr,p,size);
            else
                glBufferSubData(bufferType, 0, size, p );
        } else {
            if(ptr)
                throw std::runtime_error("Cannot change size of mappable buffer");
            glBufferData(bufferType, size, p, usage);
            this->size = size;
        }
        glBindBuffer(bufferType, 0 );
    }
    
private:
    void init(size_t size, GLenum usage, GLenum bufferType, bool mappable){
        GLuint tmp[1];
        glGenBuffers(1,tmp);
        this->buffID = tmp[0];
        this->size=size;
        glBindBuffer(bufferType, buffID);
        if( mappable ){
            glBufferStorage(bufferType, size, nullptr,
                GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | 
                GL_MAP_READ_BIT | GL_MAP_WRITE_BIT );
            ptr = glMapBufferRange(bufferType,
                0, size, 
                GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | 
                GL_MAP_READ_BIT | GL_MAP_WRITE_BIT 
            );
        } else {
            glBufferData(bufferType,size,nullptr,usage);
            this->usage = usage;
            ptr=nullptr;
        }
    }
};
        
