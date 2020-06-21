﻿using IcVibracoes.Common.Profiles;
using IcVibracoes.Core.ArrayOperations;
using IcVibracoes.Core.AuxiliarOperations.BoundaryCondition;
using IcVibracoes.Core.AuxiliarOperations.File;
using IcVibracoes.Core.Calculator.GeometricProperties.Rectangular;
using IcVibracoes.Core.Calculator.MainMatrixes.Beam.Rectangular;
using IcVibracoes.Core.Calculator.NaturalFrequency;
using IcVibracoes.Core.Calculator.Time;
using IcVibracoes.Core.Mapper;
using IcVibracoes.Core.NumericalIntegrationMethods.Newmark;

namespace IcVibracoes.Core.Operations.CalculateVibration.FiniteElement.Beam.Rectangular
{
    /// <summary>
    /// It's responsible to calculate the vibration in a rectangular beam.
    /// </summary>
    public class CalculateRectangularBeamVibration : CalculateBeamVibration<RectangularProfile>, ICalculateRectangularBeamVibration
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
        /// <param name="newmarkMethod"></param>
        /// <param name="naturalFrequency"></param>
        public CalculateRectangularBeamVibration(
            IBoundaryCondition boundaryCondition, 
            IArrayOperation arrayOperation,
            IRectangularGeometricProperty geometricProperty,
            IMappingResolver mappingResolver,
            IRectangularBeamMainMatrix mainMatrix, 
            IFile file, 
            ITime time, 
            INewmarkMethod newmarkMethod, 
            INaturalFrequency naturalFrequency) 
            : base(boundaryCondition, arrayOperation, geometricProperty, mappingResolver, mainMatrix, file, time, newmarkMethod, naturalFrequency)
        {
        }
    }
}
