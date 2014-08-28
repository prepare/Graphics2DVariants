using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
namespace SoftEngine
{
    public class WriteableBitmap
    {
        PixelBuffer pixelBuffer;
        Bitmap myBmp;
        public WriteableBitmap(int width, int height)
        {
            this.PixelWidth = width;
            this.PixelHeight = height;
            myBmp = new Bitmap(width, height);
            pixelBuffer = new PixelBuffer(width * height * 4);//rgba 
        }
        public Bitmap GetBitmap()
        {
            //ขอ bitmap
            return myBmp;
        }
        public int PixelWidth
        {
            get;
            private set;
        }
        public int PixelHeight
        {
            get;
            private set;
        }
        public void Invalidate()
        {
            //เขียน 
            BitmapData bmpdata = myBmp.LockBits(new Rectangle(0, 0, this.PixelWidth, this.PixelHeight),
                 ImageLockMode.ReadWrite, myBmp.PixelFormat);
            //จากนั้นให้ copy 
            //data ใน pixel buffer มาเขียนใน bmp
            byte[] bmpBuffer = this.pixelBuffer.GetBytes();
            System.Runtime.InteropServices.Marshal.Copy(
                bmpBuffer, 0, bmpdata.Scan0, bmpBuffer.Length);
            myBmp.UnlockBits(bmpdata);
        }
        public PixelBuffer PixelBuffer
        {
            get
            {
                return this.pixelBuffer;
            }
        }
    }
    public class PixelBuffer : IDisposable
    {
        byte[] pixelBuffer;
        bool isClosed = false;
        public PixelBuffer(int pixelBufferSize)
        {
            this.pixelBuffer = new byte[pixelBufferSize];
        }
        public void Close()
        {
            isClosed = true;
        }
        public void Dispose()
        {
            if (isClosed && pixelBuffer != null)
            { 
                pixelBuffer = null;
            }
            //if (isClosed && ms != null)
            //{
            //    ms.Close();
            //    ms.Dispose();
            //    ms = null;
            //}
        }
        public PixelBuffer AsStream()
        {
            return this;
        }
        public byte[] GetBytes()
        {
            return this.pixelBuffer;
        }
        public void Write(byte[] source, int startIndex, int length)
        {
            Buffer.BlockCopy(source, startIndex, this.pixelBuffer, 0, length);             
        }
    }
}