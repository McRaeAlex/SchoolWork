# Assignment 4

This is my submission for assignment 4

## My render

My render has the suzanne in greenish yellow, a directional light from above, a point light behind the camera, a transparent sphere, a solid phong sphere, and a mirror reflecting a sphere.

Note: the render is not 1000 x 1000 I had to run it on a laptop i did not own and did not have the time to let it run. If I do have a desktop I can render with in the future I will reload with the larger display.

## implemented

* multithreading (max threads) splits into slabs (1)
* Shadows (1)
* transparency (1)
* render mesh (1)
* glossy reflection (1)

## Multithreading

I implemented multithreading in two ways. The first I did basic para by querying for the number of supported threads and then splitting my image into n many sections and giving each thread a section.

Here is the timings for my final RenderSpeedTest.bmp which I have uploaded. It contains 2 spheres 1 triangle and one ground plane. My other scenes were taking to long.
This was done on a computer with a ryzen 1600x. Compiled with release mode.

No multithreading | 2111 miliseconds
Basic | 1181 miliseconds

It is likely the overhead of threads is what stopped this from being fasted. Another thing it could be is that my objects are bunched up meaning a couple threads had a much heavier load than the others.

## Shadows

In order to implement the shadows I just checked if there was an object between the point hit and the light and if there was I did not add the light to the output color.
This code can be found in the Phong or Matt shade function

## Transparency

In order to implement transparency I made a super class of phong then I implemented refraction using snells law. I cast the newly calculated ray into the scene and add it to the output color.
The code for this can be found in the SimpleTransparent material shade function.

## Mesh

I rendered the mesh by reading in the triangles from the obj file and then making triangles putting them in a list. When we check for a hit in the mesh we just go through all the triangles.

## Glossy Mirror Reflections

The glossy reflection were implemented by calculating the reflected vector and passing it to the BRDF which sends a ray from the hit position in the reflected direction and returns the result of that new ray.
This code can be found in both the GlossyReflection material code and the GlossySpecular BRDF.