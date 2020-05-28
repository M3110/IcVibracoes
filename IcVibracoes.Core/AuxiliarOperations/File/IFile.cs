﻿namespace IcVibracoes.Core.AuxiliarOperations.File
{
    public interface IFile
    {
        /// <summary>
        /// Writes the values ​​corresponding to an instant of time in a file.
        /// </summary>
        /// <param name="time"></param>
        /// <param name="values"></param>
        /// <param name="path"></param>
        void Write(double time, double[] values, string path);
    }
}