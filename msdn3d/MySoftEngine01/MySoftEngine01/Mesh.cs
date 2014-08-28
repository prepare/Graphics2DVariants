// Mesh.cs
using SharpDX;

namespace SoftEngine
{
    //public class Mesh
    //{
    //    public string Name { get; set; }
    //    public Vector3[] Vertices { get; private set; }
    //    public Vector3 Position { get; set; }
    //    public Vector3 Rotation { get; set; }

    //    public Mesh(string name, int verticesCount)
    //    {
    //        Vertices = new Vector3[verticesCount];
    //        Name = name;
    //    }
    //}
    public class Mesh
    {
        public string Name { get; set; }
        public Vector3[] Vertices { get; private set; }
        public Face[] Faces { get; set; }
        public Vector3 Position { get; set; }
        public Vector3 Rotation { get; set; }
        public Color4 MeshColor { get; set; }
        public Mesh(string name, int verticesCount)
        {
            Vertices = new Vector3[verticesCount];
            Name = name;
        }
        public Mesh(string name, int verticesCount, int facesCount)
        {
            Vertices = new Vector3[verticesCount];
            Faces = new Face[facesCount];
            Name = name;
        }





    }
}
