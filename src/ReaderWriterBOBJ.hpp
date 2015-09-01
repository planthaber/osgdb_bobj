


#include <osgDB/Registry>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgUtil/Optimizer>

using namespace std;

class  ReaderWriterBOBJ: public osgDB::ReaderWriter
{

	struct writeFileStruct : public osgDB::ReaderWriter::WriteResult{

	      void setStatus(osgDB::ReaderWriter::WriteResult::WriteStatus status){
	    	  _status = status;
	      }
	};

    struct readFileStruct : public osgDB::ReaderWriter::ReadResult{
      std::string fileName;

      readFileStruct(){
    	  _status = osgDB::ReaderWriter::ReadResult::FILE_LOADED;
      }

      void setObject(osg::ref_ptr<osg::Node> node){
    	  _object = node;
      }

      void setStatus(osgDB::ReaderWriter::ReadResult::ReadStatus status){
    	  _status = status;
      }

    }; // end of struct readFileStruct

	std::vector<readFileStruct> nodeFiles;


public:

	ReaderWriterBOBJ();

	virtual osgDB::ReaderWriter::ReadResult readNode (const std::string & file, const osgDB::ReaderWriter::Options *options =NULL) const;

	virtual osgDB::ReaderWriter::ReadResult readNode (std::istream &stream, const Options *options=NULL) const ;

	virtual osgDB::ReaderWriter::WriteResult writeObject (const osg::Object & object, const std::string & file, const Options *options=NULL) const;

	virtual osgDB::ReaderWriter::WriteResult writeObject (const osg::Object &object, std::ostream & stream, const Options *options=NULL) const ;

private:
	osgDB::ReaderWriter::ReadResult readBobjFromFile(const std::string & file) const;

};
