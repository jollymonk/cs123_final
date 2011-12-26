Molly Junck (mjunck)
9/28/11

Shapes README

I tried to make my program as object-oriented as possible and abstracted lots of methods into the Shape class in the shape/ folder.  I created a struct called GL_Triangle that has Vector4 coordinates of the 3 position vectors and 3 normal vectors for every triangle.  The ShapesScene keeps a variable m_shape of the current shape on the grid.  I created a method settingsChanged() in ShapesScene (and OpenGLScene) that notifies the scene anytime the settings are changed.  If the stacks, slices or shape are different, it instantiates a new shape.


Each shape computes its own geometry and calculates its own normals, but the Shape method has the main draw() method.  Each shape has a list of GL_Triangles which are all of the triangles in the mesh and draw() just iterates through the list drawing each triangle.