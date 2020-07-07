﻿using IcVibracoes.Common.Profiles;
using IcVibracoes.Core.ArrayOperations;
using IcVibracoes.Core.BoundaryCondition;
using IcVibracoes.Core.Calculator.GeometricProperties.Circular;
using IcVibracoes.Core.Calculator.MainMatrixes.BeamWithPiezoelectric.Circular;
using IcVibracoes.Core.Calculator.NaturalFrequency;
using IcVibracoes.Core.Calculator.Time;
using IcVibracoes.Core.Mapper;
using IcVibracoes.Core.Validators.Profiles.Circular;

namespace IcVibracoes.Core.Operations.CalculateVibration.FiniteElement.BeamWithPiezoelectric.Circular
{
    /// <summary>
    /// It's responsible to calculate the vibration in a circular profile beam with piezoelectric.
    /// </summary>
    public class CalculateCircularBeamWithPiezoelectricVibration : CalculateBeamWithPiezoelectricVibration<CircularProfile>, ICalculateCircularBeamWithPiezoelectricVibration
    {
        /// <summary>
        /// Class constructor.
        /// </summary>
        /// <param name="boundaryCondition"></param>
        /// <param name="arrayOperation"></param>
        /// <param name="geometricProperty"></param>
        /// <param name="mappingResolver"></param>
        /// <param name="mainMatrix"></param>
        /// <param name="profileValidator"></param>
        /// <param name="time"></param>
        /// <param name="naturalFrequency"></param>
        public CalculateCircularBeamWithPiezoelectricVibration(
            IBoundaryCondition boundaryCondition, 
            IArrayOperation arrayOperation,
            ICircularGeometricProperty geometricProperty, 
            IMappingResolver mappingResolver, 
            ICircularBeamWithPiezoelectricMainMatrix mainMatrix,
            ICircularProfileValidator profileValidator, 
            ITime time, 
            INaturalFrequency naturalFrequency) 
            : base(boundaryCondition, arrayOperation, geometricProperty, mappingResolver, mainMatrix, profileValidator, time, naturalFrequency)
        { }
    }
}
