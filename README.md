# Camera Activity

In this activity, you will create a Camera class to help understand the synthetic camera properties and the view and projection matrices.

## To Do

Open the Camera class and complete the following methods:
- lookAt
- getView
- getProjection

However, you may not use the following GLM functions for this activity:
- glm::lookAt(...)
- glm::perspective(...)

Make sure to commit after implementing each method and push when the activity is complete!

### Hints

- Remember in GLM and OpenGL, matrices are specified in column major order in the constructor.
- The [GLM matrix access](https://glm.g-truc.net/0.9.3/api/a00144.html) functions may be helpful to set parts of the view/projection matrices.
- The U, V, and N axes should always be orthogonal and normalized! In addition, the U, V,
and N axes are not necessarily the same as your forward and up vectors.

### Optional Extras

The starter code already includes code that rotates the camera using a trackball metaphor. If you still have time after completing the tasks above, add code that lets you dolly the camera forward or backward along the look direction and left or right along the U axis when the A, S, W, or D keys are held down.

MinVR only gives keyboard events for when a key is pressed down or released up. You will need to add some variables in the code to keep track of which buttons are pressed. In the move() method you can update the position based on the pressed buttons.




