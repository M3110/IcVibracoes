﻿using IcVibracoes.Common.Profiles;
using IcVibracoes.Core.ArrayOperations;
using IcVibracoes.Core.AuxiliarOperations.BoundaryCondition;
using IcVibracoes.Core.AuxiliarOperations.File;
using IcVibracoes.Core.Calculator.GeometricProperties.Rectangular;
using IcVibracoes.Core.Calculator.MainMatrixes.BeamWithDva.Rectangular;
using IcVibracoes.Core.Calculator.NaturalFrequency;
using IcVibracoes.Core.Calculator.Time;
using IcVibracoes.Core.Mapper;

namespace IcVibracoes.Core.Operations.CalculateVibration.FiniteElement.BeamWithDva.Rectangular
{
    /// <summary>
    /// It's responsible to calculate the vibration in a rectangular beam with dynamic vibration absorber.
    /// </summary>
    public class CalculateRectangularBeamWithDvaVibration : CalculateBeamWithDvaVibration<RectangularProfile>, ICalculateRectangularBeamWithDvaVibration
    {
        /// <summary>
        /// Class constructor.
        /// </summary>
        /// <param name="boundaryCondition"></param>
        /// <param name="arrayOperation"></param>
        /// <param name="geometricProperty"></param>
        /// <param name="mappingResolver"></param>
        /// <param name="mainMatrix"></param>
        /// <param name="file"></param>
        /// <param name="time"></param>
        /// <param name="naturalFrequency"></param>
        public CalculateRectangularBeamWithDvaVibration(
            IBoundaryCondition boundaryCondition,
            IArrayOperation arrayOperation, IRectangularGeometricProperty geometricProperty,
            IMappingResolver mappingResolver,
            IRectangularBeamWithDvaMainMatrix mainMatrix,
            IFile file,
            ITime time,
            INaturalFrequency naturalFrequency)
            : base(boundaryCondition, arrayOperation, geometricProperty, mappingResolver, mainMatrix, file, time, naturalFrequency)
        { }
    }
}