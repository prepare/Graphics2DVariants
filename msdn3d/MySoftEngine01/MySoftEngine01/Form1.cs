using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;

using System.Text;
using System.Windows.Forms;
using SharpDX;

namespace SoftEngine
{
    public partial class Form1 : Form
    {
        private Device device;

        Camera camera = new Camera();
        WriteableBitmap bmp = new WriteableBitmap(640, 480);
        List<Mesh> meshList = new List<Mesh>();

        public Form1()
        {
            InitializeComponent();

        }
        void Setup1()
        {

            //device = new Device(bmp);
            //// Our Image XAML control 
            //frontBuffer.Image = bmp.GetBitmap();
            //mesh.Vertices[0] = new Vector3(-1, 1, 1);
            //mesh.Vertices[1] = new Vector3(1, 1, 1);
            //mesh.Vertices[2] = new Vector3(-1, -1, 1);
            //mesh.Vertices[3] = new Vector3(-1, -1, -1);
            //mesh.Vertices[4] = new Vector3(-1, 1, -1);
            //mesh.Vertices[5] = new Vector3(1, 1, -1);
            //mesh.Vertices[6] = new Vector3(1, -1, 1);
            //mesh.Vertices[7] = new Vector3(1, -1, -1);

            //camera.Position = new Vector3(0, 0, 10.0f);
            //camera.Target = Vector3.Zero; 
        }
        void SetupPlain(Vector3 leftUpperCorner, float width, float height, Vector3 angleVec1, Vector3 angleVec2, Color4 color)
        {
            //left upper -corner

            {
                Mesh plain1 = GeoHelper.CreateCube(0.02f);
                plain1.Position = leftUpperCorner; //left corner
                plain1.MeshColor = color;
                plain1.Rotation = new Vector3(.5f, 0.5f, 0);
                meshList.Add(plain1);
            }
            //right upper corner
            //คือวิ่งตาม row
            var x = angleVec1.X;//Math.Cos(angleVec1.X);
            var y = angleVec1.Y;//Math.Cos(angleVec1.Y);
            var z = angleVec1.Z;// Math.Cos(angleVec1.Z);
            //var x = Math.Cos(angleVec1.X);
            //var y = Math.Cos(angleVec1.Y);
            //var z = Math.Cos(angleVec1.Z);
            {
                Mesh plain1 = GeoHelper.CreateCube(0.02f);
                plain1.Position = leftUpperCorner + new Vector3((float)(x * width), (float)(y * width), (float)(z * width)); //left corner
                plain1.MeshColor = color;
                plain1.Rotation = new Vector3(.5f, 0.5f, 0);
                meshList.Add(plain1);
            }
            //x = Math.Cos(angleVec2.X);
            //y = Math.Cos(angleVec2.Y);
            //z = Math.Cos(angleVec2.Z);
            x = angleVec2.X;//Math.Cos(angleVec1.X);
            y = angleVec2.Y;//Math.Cos(angleVec1.Y);
            z = angleVec2.Z;// Math.Cos(angleVec1.Z);

            {
                Mesh plain1 = GeoHelper.CreateCube(0.02f);
                plain1.Position = leftUpperCorner + new Vector3((float)(x * width), (float)(y * width), (float)(z * width)); //left corner
                plain1.MeshColor = color;
                plain1.Rotation = new Vector3(.5f, 0.5f, 0);
                meshList.Add(plain1);
            }

            Vector3 a3 = angleVec1 + angleVec2;
            x = a3.X;//Math.Cos(angleVec1.X);
            y = a3.Y;//Math.Cos(angleVec1.Y);
            z = a3.Z;// Math.Cos(angleVec1.Z);

            {
                Mesh plain1 = GeoHelper.CreateCube(0.02f);
                plain1.Position = leftUpperCorner + new Vector3((float)(x * width), (float)(y * width), (float)(z * width)); //left corner
                plain1.MeshColor = color;
                plain1.Rotation = new Vector3(.5f, 0.5f, 0);
                meshList.Add(plain1);
            }
            //left lower corner
            //วิ่งลงตาม column

            //right lower corner
            // 
        }
        void Setup2()
        {

            //--------------------------------- 
            float maxRefBoxLength = 1.0f;
            Mesh referenceMesh = GeoHelper.CreateCube(maxRefBoxLength);
            referenceMesh.MeshColor = new Color4(1.0f, 1.0f, 0.0f, 1.0f);
            meshList.Add(referenceMesh);
            //---------------------------------
            //reference node
            {
                Mesh plain1 = GeoHelper.CreateCube(0.02f);//, 0.02f, 0.02f);
                plain1.Position = new Vector3(maxRefBoxLength, 0f, 0f); //x
                plain1.MeshColor = new Color4(1.0f, 1.0f, 1.0f, 1.0f);
                plain1.Rotation = new Vector3(.5f, 0.5f, 0);
                meshList.Add(plain1);
            }
            {
                Mesh plain1 = GeoHelper.CreateCube(0.02f);//, 0.02f, 0.02f);
                plain1.Position = new Vector3(0, maxRefBoxLength, 0f); //y
                plain1.MeshColor = new Color4(1.0f, .0f, 1.0f, 1.0f);//margenta
                plain1.Rotation = new Vector3(.5f, 0.5f, 0);
                meshList.Add(plain1);
            }
            {
                Mesh plain1 = GeoHelper.CreateCube(0.02f);//, 0.02f, 0.02f);
                plain1.Position = new Vector3(0, 0, maxRefBoxLength); //z
                plain1.MeshColor = new Color4(0.0f, 1.0f, 1f, 1.0f);
                plain1.Rotation = new Vector3(.5f, 0.5f, 0);
                meshList.Add(plain1);
            }
            //Mesh mesh2 = GeoHelper.CreateCube(0.2f, 0.5f, 0.01f);
            //Mesh mesh2 = GeoHelper.CreateCube(0.02f, 0.02f, 0.02f);
            //mesh2.Position = new Vector3(0.5f, 0.5f, 0);
            //mesh2.MeshColor = new Color4(1.0f, 0.0f, 0.0f, 1.0f); 
            //mesh2.Rotation = new Vector3(.5f, 0.5f, 0);
            //meshList.Add(mesh2);
            //---------------------------------
            //destination
            {
                float y = float.Parse("6.123032E-17");
                SetupPlain(
                    new Vector3(-0.2555f, -0.1375f, 1.072f),
                    0.6f, 0.6f,
                    new Vector3(1, y, 0),//row
                    new Vector3(0, 0, -1),//col
                    new Color4(1.0f, 1.0f, 1.0f, 1.0f));
            }
            //---------------------------------


            //{
            //    Mesh plain1 = GeoHelper.CreateCube(0.02f);//, 0.02f, 0.02f);
            //    plain1.Position = new Vector3(-0.2555f, -0.1375f, 1.072f); //left corner
            //    plain1.MeshColor = new Color4(1.0f, 1.0f, 1.0f, 1.0f); //red
            //    plain1.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain1);
            //}
            //{
            //    Mesh plain1 = GeoHelper.CreateCube(0.02f);//, 0.02f, 0.02f);
            //    plain1.Position = new Vector3(-0.2555f, -0.1375f, 0.672000051f); //z-.4
            //    plain1.MeshColor = new Color4(1.0f, .3f, 0.0f, 1.0f); //red
            //    plain1.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain1);
            //}
            //{
            //    Mesh plain1 = GeoHelper.CreateCube(0.02f);//, 0.02f, 0.02f); 
            //    plain1.Position = new Vector3(0.144500017f, -0.1375f, 1.072f); //x+.4
            //    plain1.MeshColor = new Color4(1.0f, .3f, 0.0f, 1.0f); //red
            //    plain1.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain1);
            //}
            //{
            //    Mesh plain1 = GeoHelper.CreateCube(0.02f);//, 0.02f, 0.02f);
            //    plain1.Position = new Vector3(0.144500017f, -0.1375f, 0.672000051f);//x+.4,z-.4
            //    plain1.MeshColor = new Color4(1.0f, .3f, 0.0f, 1.0f); //red
            //    plain1.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain1);
            //}

            //---------------------------------
            //source
            {
               
                //plain2.Position = new Vector3(-0.182642578f, -0.320142578f, 1.0405f);
                SetupPlain(
                    new Vector3(-0.182642578f, -0.320142578f, 1.0405f),
                    0.4f, 0.4f,
                    new Vector3(1, 0, 0),//row
                    new Vector3(0, 1, 0),//col
                    new Color4(1.0f, .3f, 0.0f, 1.0f));
            }
            //for (int i = 0; i < 10; ++i)
            //{
                
            //    //plain2.Position = new Vector3(-0.182642578f, -0.320142578f, 1.0405f);
            //    SetupPlain(
            //        new Vector3(-0.182642578f, -0.320142578f, 1.0405f - ((float)i * 0.04f)),
            //        0.4f, 0.4f,
            //        new Vector3(1, 0, 0),//row
            //        new Vector3(0, 1, 0),//col
            //        new Color4(0.0f, 1f, 0.0f, 1.0f));
            //}
            //{
            //    Mesh plain2 = GeoHelper.CreateCube(0.02f);
            //    plain2.Position = new Vector3(-0.182642578f, -0.320142578f, 1.0405f);
            //    plain2.MeshColor = new Color4(0.0f, 1.0f, 0.0f, 1.0f);//green
            //    plain2.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain2);
            //}
            //{
            //    Mesh plain2 = GeoHelper.CreateCube(0.02f);
            //    plain2.Position = new Vector3(0.217357427f, -0.320142578f, 1.0405f); //x+.4
            //    plain2.MeshColor = new Color4(0.0f, 1.0f, 0.0f, 1.0f);//green
            //    plain2.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain2);
            //}
            //{
            //    Mesh plain2 = GeoHelper.CreateCube(0.02f);
            //    plain2.Position = new Vector3(-0.182642578f, 0.07985744f, 1.0405f); //y+.4
            //    plain2.MeshColor = new Color4(0.0f, 1.0f, 0.0f, 1.0f);//green
            //    plain2.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain2);
            //}
            //{
            //    Mesh plain2 = GeoHelper.CreateCube(0.02f);
            //    plain2.Position = new Vector3(0.217357427f, 0.07985744f, 1.0405f);//x+.4,z+.4
            //    plain2.MeshColor = new Color4(0.0f, 1.0f, 0.0f, 1.0f);//green
            //    plain2.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain2);
            //}

            //---------------------------------
            //source2
            //{
            //    float y = float.Parse("6.123032E-17");
            //    //plain2.Position = new Vector3(-0.182642578f, -0.320142578f, 1.0405f);
            //    SetupPlain(
            //       new Vector3(-0.182642578f, -0.320142578f, 0.6055f),
            //        0.4f, 0.4f,
            //        new Vector3(1, y, 0),//row
            //        new Vector3(0, 0, -1),//col
            //        new Color4(0.0f, 0.0f, 1.0f, 1.0f));
            //}
            //{
            //    Mesh plain3 = GeoHelper.CreateCube(0.02f);
            //    plain3.Position = new Vector3(-0.182642578f, -0.320142578f, 0.6055f);//1.0255f
            //    plain3.MeshColor = new Color4(0.0f, 0.0f, 1.0f, 1.0f);//blue
            //    plain3.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain3);
            //}
            //{
            //    Mesh plain3 = GeoHelper.CreateCube(0.02f);
            //    plain3.Position = new Vector3(0.217357427f, -0.320142578f, 0.6055f); //x+.4
            //    plain3.MeshColor = new Color4(0.0f, 0.0f, 1.0f, 1.0f);//blue
            //    plain3.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain3);
            //}
            //{
            //    Mesh plain3 = GeoHelper.CreateCube(0.02f);
            //    plain3.Position = new Vector3(-0.182642578f, 0.07985744f, 0.6055f); //y+.4
            //    plain3.MeshColor = new Color4(0.0f, 0.0f, 1.0f, 1.0f);//blue
            //    plain3.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain3);
            //}
            //{
            //    Mesh plain3 = GeoHelper.CreateCube(0.02f);
            //    plain3.Position = new Vector3(0.217357427f, 0.07985744f, 0.6055f);//x+.4,y+.4
            //    plain3.MeshColor = new Color4(0.0f, 0.0f, 1.0f, 1.0f);//blue
            //    //plain3.Rotation = new Vector3(.5f, 0.5f, 0);
            //    meshList.Add(plain3);
            //}
            //---------------------------------
            //camera.Position = new Vector3(0f, 0.0f, 8.0f);
            //x เป็นตำแหน่งกึ่งกลางแกน x 
            camera.Target = new Vector3(-0.2555f + .2f, -0.6375f + .2f, 1.072f + 0.0001f);
            camera.Rotate = Matrix.Identity;// Matrix.RotationX((float)DegToRad(-8));
            camera.Translate = Matrix.Identity;
            //---------------------------------

            //camera.Rotate = Matrix.RotationZ((float)DegToRad(30));
            //เคลื่อน camera ออกไปเป็นมุมฉาก 
            //camera.Position = new Vector3(-0.2555f + .2f,
            //   -0.6375f + .2f,
            //   1.072f + 7); //+7

            float angle_seta = (float)DegToRad(90);
            float angle_alpha = (float)DegToRad(0);
            float angle_beta = (float)DegToRad(0);

            float c_x = -0.2555f + .2f;
            float c_y = -0.6375f + .2f;
            float c_z = 1.072f + .2f;

            float d = 7;

            //float finalpos_x = (float)((d * Math.Cos(angle_seta) * Math.Sin(angle_beta)) + (d * Math.Sin(angle_seta) * Math.Sin(angle_alpha)));
            //float finalpos_y = (float)((d * Math.Sin(angle_seta)) + (d * Math.Sin(angle_seta) * Math.Cos(angle_alpha)));
            //float finalpos_z = (float)(d * Math.Cos(angle_seta) * Math.Cos(angle_beta));
            //float d = 7f;
            //float finalpos_x = (float)((d * Math.Cos(angle_seta) * Math.Sin(angle_beta)) + (d * Math.Sin(angle_seta) * Math.Sin(angle_alpha)));

            //float finalpos_y = (float)((d * Math.Sin(angle_seta)));//+ (d * Math.Sin(angle_seta) * Math.Cos(angle_alpha)));            

            float finalpos_x = (float)(d * Math.Cos(angle_seta) * Math.Sin(angle_beta));
            float finalpos_y = (float)(d * Math.Sin(angle_seta));
            float finalpos_z = (float)(d * Math.Cos(angle_seta) * Math.Cos(angle_beta));

            //camera.Translate = Matrix.Translation(0, finalpos_y, finalpos_z);
            //Vector3 v1 = new Vector3(c_x + finalpos_x,
            //    c_y + finalpos_y,
            //    c_z + finalpos_z);
            Vector3 v1 = new Vector3(c_x + finalpos_x, c_y + finalpos_y, c_z + finalpos_z);
            camera.Position = v1;

            //camera.Translate = Matrix.Translation(0, c_y + finalpos_y, c_z + finalpos_z);

            //Vector3 v2 = new Vector3(c_x,
            //    c_y - (float)(Math.Sin(Math.PI * angle_seta / 180) * d),
            //    c_z - ((float)(Math.Cos(Math.PI * angle_seta / 180) * d)));

            
            //camera.Position = new Vector3(c_x,
            //    c_y - (float)(Math.Sin(Math.PI * angle_seta / 180) * d),
            //    c_z - ((float)(Math.Cos(Math.PI * angle_seta / 180) * d))); //+7



            //camera.Position = new Vector3(-0.2555f + .2f, -0.6375f + .2f, 1.072f + (float)(Math.Cos(Math.PI * 45 / 180) * 7)); //+7

            //camera.Position = new Vector3(-0.2555f + .2f, -6.6375f + .2f, 8.072f);
            //camera.Position = new Vector3(-0.2555f + .2f, -8.6375f + .2f, 6.072f);
            //camera.Position = new Vector3(-0.2555f + .2f, -10.6375f + .2f, 4.072f);
            //camera.Position = new Vector3(-0.2555f + .2f, -12.6375f + .2f, 2.072f);
            //camera.Position = new Vector3(-0.2555f + .2f, -14.6375f + .2f, 2.072f);
            //camera.Position = new Vector3(-0.2555f + .2f, -16.6375f + .2f, 2.072f);
            ////---------------------------------

            //camera.Position = new Vector3(-0.2555f + .2f, -11.6375f + .2f, 1.072f);
            ////camera.Position = new Vector3(-0.2555f + .2f, -10.6375f, 4.072f);
            //camera.Position = new Vector3(-0.2555f + .2f, -16.6375f + .2f, 1.22f);
            // camera.Position = new Vector3(-0.2555f + .2f, -16.6375f  , 1.075f);
        }
        static double DegToRad(double degree)
        {
            return Math.PI * degree / 180;
        }
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            device = new Device(bmp);
            Setup2();
            this.timer1.Tick += new EventHandler(timer1_Tick);
            this.timer1.Interval = 50;
            this.timer1.Enabled = true;
        }

        void timer1_Tick(object sender, EventArgs e)
        {
            UpdateRendering();
        }

        void UpdateRendering()
        {

            device.Clear(0, 0, 0, 255);

            // rotating slightly the cube during each frame rendered
            //mesh.Rotation = new Vector3(mesh.Rotation.X + 0.01f, mesh.Rotation.Y + 0.01f, mesh.Rotation.Z);
         
            //mesh.Position = new Vector3(mesh.Position.X, mesh.Position.Y, mesh.Position.Z + 0.01f);

            // Doing the various matrix operations
            int j = this.meshList.Count;
            for (int i = 0; i < j; ++i)
            {
                device.Render(camera, meshList[i]);
            }

            // Flushing the back buffer into the front buffer
            device.Present();

            frontBuffer.Image = bmp.GetBitmap();
        }


    }
}
