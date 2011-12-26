Molly Junck (mjunck)

Sceneview README

To store the objects and transformations, I made a scene_obj struct in Scene.h that contained the transformation matrix, the material and the object type.  To store all of the scene_objs and the lights, I used std::lists.  In Scene.cpp, I wrote a flattenTree(...) method that recursively takes a node from the parser, creates a scene_obj storing all the transformations necessary, and adds the scene_obj and the light to their respective lists.  It passes the current transformation matrix, etc., to the child nodes and recurses.

In Sceneview.cpp, I avoid storing an entire object and its info at every node in the graph by instantiating one of each primitive shape (cube, cylinder, sphere, cone) in Sceneview.cpp and kept redrawing those.

Since it's an optional debugging feature, my renderNormals() is a little hacky.  Instead of rendering all of the normals together, if renderNormals is true, I render normals shape by shape as the geometry is being rendered (with renderNormals(Shape s)).

The other thing which is annoying is that I haven't fixed the settingsChanged() method in Sceneview.cpp, i.e. if you load a new scene while you are currently in a scene, it won't render until you click on something.  I am sure there is a really obvious way to make the Canvas3D redraw it/create a new Sceneview object, but I haven't seen it yet.
