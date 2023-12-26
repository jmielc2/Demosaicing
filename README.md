# Demosaicing Project
A small project where I apply a demosaicing algorithm to a mosaiced image to create a full RGB image similar to the original. Demosaicing is the process of reconstructing an image from what is essentially a third of the information (each pixel only contains one channel of color information whereas a full RGB pixel would contain three). This process isn't a perfect reconstruction as can be seen in the example below.
Implemented in C++ with the help of the SDL2 and SDL_image libraries.

# Original:
![alt text](https://github.com/jmielc2/Demosaicing/blob/master/docs/shinji.jpg?raw=true)

# Intermediate (Mosaiced):
![alt text](https://github.com/jmielc2/Demosaicing/blob/master/docs/intermediate.png?raw=true)

# Result (Demosaiced):
![alt text](https://github.com/jmielc2/Demosaicing/blob/master/docs/result.png?raw=true)

Notice the artifacts in the silhouette, expecially the hair.
