# Demosaicing Project
A small project where I apply a demosaicing algorithm to a mosaiced image to create a full RGB image similar to the original. Demosaicing is the process of reconstructing an image from what is essentially a third of the information (each pixel only contains one channel of color information whereas a full RGB pixel would contain three). This process isn't a perfect reconstruction as can be seen in the example below.
Implemented in C++ with the help of the SDL2 and SDL_image libraries.

| Original | Intermediate (Mosaiced) | Demosaiced |
| --- | --- | --- |
| ![](/docs/shinji.jpg) | ![](/docs/intermediate.png) | ![](/docs/result.png) |

Notice the artifacts in the silhouette, expecially the hair.
