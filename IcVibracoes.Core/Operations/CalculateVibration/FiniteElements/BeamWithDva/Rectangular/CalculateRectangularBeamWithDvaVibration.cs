﻿using IcVibracoes.Common.Profiles;
using IcVibracoes.Core.ArrayOperations;
using IcVibracoes.Core.AuxiliarOperations;
using IcVibracoes.Core.Calculator.GeometricProperties.Rectangular;
using IcVibracoes.Core.Calculator.MainMatrixes.BeamWithDva.Rectangular;
using IcVibracoes.Core.Calculator.Time;
using IcVibracoes.Core.DTO.NumericalMethodInput.FiniteElements;
using IcVibracoes.Core.Mapper;
using IcVibracoes.Core.NumericalIntegrationMethods.Newmark;
using IcVibracoes.Core.Validators.Profiles.Rectangular;

namespace IcVibracoes.Core.Operations.CalculateVibration.FiniteElements.BeamWithDva.Rectangular
{
    /// <summary>
    /// It's responsible to calculate the vibration in a rectangular beam with dynamic vibration absorber.
    /// </summary>
    public class CalculateRectangularBeamWithDvaVibration : CalculateBeamWithDvaVibration<RectangularProfile, NewmarkMethodInput>, ICalculateRectangularBeamWithDvaVibration
    {
        /// <summary>
        /// Class constructor.
        /// </summary>
        /// <param name="newmarkMethod"></param>
        /// <param name="profileValidator"></param>
        /// <param name="auxiliarOperation"></param>
        /// <param name="time"></param>
        /// <param name="arrayOperation"></param>
        /// <param name="geometricProperty"></param>
        /// <param name="mappingResolver"></param>
        /// <param name="mainMatrix"></param>
        public CalculateRectangularBeamWithDvaVibration(
            INewmarkMethod newmarkMethod,
            IRectangularProfileValidator profileValidator,
            IAuxiliarOperation auxiliarOperation,
            ITime time,
            IArrayOperation arrayOperation,
            IRectangularGeometricProperty geometricProperty,
            IMappingResolver mappingResolver,
            IRectangularBeamWithDvaMainMatrix mainMatrix)
            : base(newmarkMethod, profileValidator, auxiliarOperation, time, arrayOperation, geometricProperty, mappingResolver, mainMatrix)
        {
        }
    }
}
