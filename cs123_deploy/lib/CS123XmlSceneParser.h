/*!
   @file   CS123XmlSceneParser.h
   @author Eric Tamura (October 2006)
   @author Nong Li (August 2007)

   @brief  This parser is designed to replace the aging Scenefile Parser built
           with Flex/Yacc/Bison.  It is intended to be linked to the TinyXML
           parser library that should be included with these files.
*/

#ifndef __CS123XMLSCENEPARSER__
#define __CS123XMLSCENEPARSER__

#include "CS123ISceneParser.h"
#include "CS123SceneData.h"

#include <vector>
#include <map>

class TiXmlElement;


//! This class parses the scene graph specified by the CS123 Xml file format.
class CS123XmlSceneParser : public CS123ISceneParser 
{
   public:
      //! Create a parser, passing it the scene file.
      CS123XmlSceneParser(const std::string& filename);
      //! Clean up all data for the scene
      virtual ~CS123XmlSceneParser();

      //! Parse the scene.  Returns false if scene is invalid.
      virtual bool parse();

      //! Returns global scene data
      virtual bool getGlobalData(CS123SceneGlobalData& data) const;

      //! Returns camera data
      virtual bool getCameraData(CS123SceneCameraData& data) const;

      //! Returns the root scenegraph node
      virtual CS123SceneNode* getRootNode() const;

      //! Returns number of lights in the scene
      virtual int getNumLights() const;

      //! Returns the ith light data
      virtual bool getLightData(int i, CS123SceneLightData& data) const;


   private:
      //the filename should be contained within this parser implementation
      //if you want to parse a new file, instantiate a different parser
      bool parseGlobalData(const TiXmlElement* scenefile);
      bool parseCameraData(const TiXmlElement* scenefile);
      bool parseLightData(const TiXmlElement* scenefile);
      bool parseObjectData(const TiXmlElement* scenefile);
      bool parseTransBlock(const TiXmlElement* transblock,
                           CS123SceneNode* node);
      bool parsePrimitive(const TiXmlElement* prim,
                          CS123SceneNode* node);
      
      std::string file_name;
      mutable std::map<std::string, CS123SceneNode*> m_objects;
      CS123SceneCameraData m_cameraData;
      std::vector<CS123SceneLightData*> m_lights;
      CS123SceneGlobalData m_globalData;
      std::vector<CS123SceneNode*> m_nodes;
};

#endif

