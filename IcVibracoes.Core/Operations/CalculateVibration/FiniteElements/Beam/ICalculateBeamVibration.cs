﻿using IcVibracoes.Common.Profiles;
using IcVibracoes.Core.Models.Beams;
using IcVibracoes.DataContracts.FiniteElements.Beam;

namespace IcVibracoes.Core.Operations.CalculateVibration.FiniteElements.Beam
{
    /// <summary>
    /// It's responsible to calculate the vibration in a beam.
    /// </summary>
    /// <typeparam name="TProfile"></typeparam>
    public interface ICalculateBeamVibration<TProfile> : ICalculateVibration_FiniteElements<BeamRequest<TProfile>, TProfile, Beam<TProfile>>
        where TProfile : Profile, new()
    {
    }
}
