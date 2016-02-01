
#include "ReaderWriterBOBJ.hpp"

#include <stdio.h>

using namespace std;


ReaderWriterBOBJ::ReaderWriterBOBJ(){
    supportsExtension("bobj","Binary obj format");
}


osgDB::ReaderWriter::ReadResult ReaderWriterBOBJ::readNode(
        const std::string& file,
        const osgDB::ReaderWriter::Options* options) const
{
    std::ifstream stream (file.c_str(), std::ifstream::binary);
    if(!stream.is_open()){
        readFileStruct newNodeFile;
        newNodeFile.setStatus(osgDB::ReaderWriter::ReadResult::FILE_NOT_FOUND);
        return newNodeFile;
    }
    return readBobjFromStream(stream,file);
}

osgDB::ReaderWriter::ReadResult ReaderWriterBOBJ::readNode(
        std::istream& stream, const Options* options) const {
    return readBobjFromStream(stream,"from stream");
}

osgDB::ReaderWriter::WriteResult ReaderWriterBOBJ::writeObject(
        const osg::Object& object, const std::string& file,
        const Options* options) const {
    writeFileStruct newNodeFile;
    newNodeFile.setStatus(osgDB::ReaderWriter::WriteResult::NOT_IMPLEMENTED);
    return newNodeFile;
}

osgDB::ReaderWriter::WriteResult ReaderWriterBOBJ::writeObject(
        const osg::Object& object, std::ostream& stream,
        const Options* options) const {
    writeFileStruct newNodeFile;
    newNodeFile.setStatus(osgDB::ReaderWriter::WriteResult::NOT_IMPLEMENTED);
    return newNodeFile;
}



osgDB::ReaderWriter::ReadResult ReaderWriterBOBJ::readBobjFromStream(std::istream &stream, const std::string& name) const {

    //		for(std::vector<readFileStruct>::const_iterator iter = nodeFiles.begin();
    //			iter != nodeFiles.end(); iter++) {
    //		  if((*iter).fileName == file) return (*iter);
    //		}

    readFileStruct newNodeFile;
    newNodeFile.fileName = name;

    char buffer[312];

    int da, i, r, o, foo=0;
    int iData[3];
    float fData[4];

    osg::Geode *geode = new osg::Geode();
    std::vector<osg::Vec3> vertices;
    std::vector<osg::Vec3> normals;
    std::vector<osg::Vec2> texcoords;

    std::vector<osg::Vec3> vertices2;
    std::vector<osg::Vec3> normals2;
    std::vector<osg::Vec2> texcoords2;

    osg::ref_ptr<osg::Vec3Array> osgVertices = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> osgTexcoords = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec3Array> osgNormals = new osg::Vec3Array();

    while( true ) {

        stream.read(buffer+foo, 256);
        r = stream.gcount();
        if (!r > 0){
            break;
        }

        o = 0;
        while(o < r+foo-50 || (r<256 && o < r+foo)) {
            da = *(int*)(buffer+o);
            o += 4;
            if(da == 1) {
                for(i=0; i<3; i++) {
                    fData[i] = *(float*)(buffer+o);
                    o+=4;
                }
                vertices.push_back(osg::Vec3(fData[0], fData[1], fData[2]));
            }
            else if(da == 2) {
                for(i=0; i<2; i++) {
                    fData[i] = *(float*)(buffer+o);
                    o+=4;
                }
                texcoords.push_back(osg::Vec2(fData[0], fData[1]));
            }
            else if(da == 3) {
                for(i=0; i<3; i++) {
                    fData[i] = *(float*)(buffer+o);
                    o+=4;
                }
                normals.push_back(osg::Vec3(fData[0], fData[1], fData[2]));
            }
            else if(da == 4) {
                for(i=0; i<3; i++) {
                    iData[i] = *(int*)(buffer+o);
                    o+=4;
                }
                // add osg vertices etc.
                osgVertices->push_back(vertices[iData[0]-1]);
                vertices2.push_back(vertices[iData[0]-1]);
                if(iData[1] > 0) {
                    osgTexcoords->push_back(texcoords[iData[1]-1]);
                    texcoords2.push_back(texcoords[iData[1]-1]);
                }
                osgNormals->push_back(normals[iData[2]-1]);
                normals2.push_back(normals[iData[2]-1]);

                for(i=0; i<3; i++) {
                    iData[i] = *(int*)(buffer+o);
                    o+=4;
                }
                // add osg vertices etc.
                osgVertices->push_back(vertices[iData[0]-1]);
                vertices2.push_back(vertices[iData[0]-1]);
                if(iData[1] > 0) {
                    osgTexcoords->push_back(texcoords[iData[1]-1]);
                    texcoords2.push_back(texcoords[iData[1]-1]);
                }
                osgNormals->push_back(normals[iData[2]-1]);
                normals2.push_back(normals[iData[2]-1]);

                for(i=0; i<3; i++) {
                    iData[i] = *(int*)(buffer+o);
                    o+=4;
                }
                // add osg vertices etc.
                osgVertices->push_back(vertices[iData[0]-1]);
                vertices2.push_back(vertices[iData[0]-1]);
                if(iData[1] > 0) {
                    osgTexcoords->push_back(texcoords[iData[1]-1]);
                    texcoords2.push_back(texcoords[iData[1]-1]);
                }
                osgNormals->push_back(normals[iData[2]-1]);
                normals2.push_back(normals[iData[2]-1]);
            }
        }
        foo = r+foo-o;
        if(r==256) memcpy(buffer, buffer+o, foo);
    }

#ifdef USE_MARS_VBO
    MarsVBOGeom *geometry = new MarsVBOGeom();
    geometry->setVertexArray(vertices2);
    geometry->setNormalArray(normals2);
    if(osgTexcoords->size() > 0)
        geometry->setTexCoordArray(texcoords2);
#else
    osg::Geometry* geometry = new osg::Geometry;
    geometry->setVertexArray(osgVertices.get());
    geometry->setNormalArray(osgNormals.get());
    geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    if(osgTexcoords->size() > 0)
        geometry->setTexCoordArray(0, osgTexcoords.get());

    osg::DrawArrays* drawArrays = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,osgVertices->size());
    geometry->addPrimitiveSet(drawArrays);
#endif
    geode->addDrawable(geometry);
    geode->setName("bobj" + name);



    osgUtil::Optimizer optimizer;
    optimizer.optimize( geode );

    newNodeFile.setObject(geode);
    //nodeFiles.push_back(newNodeFile);

    return newNodeFile;
}

// now register with Registry to instantiate the above
// reader/writer.
REGISTER_OSGPLUGIN(bobj, ReaderWriterBOBJ)
