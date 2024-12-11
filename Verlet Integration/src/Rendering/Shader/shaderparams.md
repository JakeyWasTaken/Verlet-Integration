## Comprehensive list of all layouts and uniforms passed into a shader

### Layouts
|    Name    | Type | Location |    Description   |
| ---------- | ---- | -------- | ---------------- |
| aPos       | vec3 | 0        | Vertex Position  |
| aNormal    | vec3 | 1        | Vertex Normal	  |
| aVertColor | vec3 | 2        | Vertex Color     |
| aTexCoords | vec2 | 3        | Vertex TexCoords |

### Uniforms
|     Name    | Type |     Description     |
| ----------- | ---- | ------------------- |
| model       | mat4 | Model Matrix        |
| view        | mat4 | View Matrix	       |
| projection  | mat4 | Projection Matrix   |
| color       | vec3 | Object Color        |
| ambient     | vec3 | Ambient World Color |
| sunPosition | vec3 | Sun World Position  |