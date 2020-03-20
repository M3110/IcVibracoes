﻿using IcVibracoes.Common.Profiles;
using IcVibracoes.Models.Beam.Characteristics;
using System.Threading.Tasks;

namespace IcVibracoes.Core.Mapper.PiezoelectricProfiles
{
    /// <summary>
    /// It's responsible to build the piezoelectric profile.
    /// </summary>
    /// <typeparam name="TProfile"></typeparam>
    public abstract class PiezoelectricProfileMapper<TProfile> : IPiezoelectricProfileMapper<TProfile>
        where TProfile : Profile, new()
    {
        /// <summary>
        /// Method to build the piezoelectric profile.
        /// </summary>
        /// <param name="piezoelectricProfile"></param>
        /// <param name="beamProfile"></param>
        /// <param name="numberOfPiezoelectricsPerElements"></param>
        /// <param name="elementsWithPiezoelectric"></param>
        /// <param name="numberOfElements"></param>
        /// <returns></returns>
        public abstract Task<GeometricProperty> Execute(TProfile piezoelectricProfile, TProfile beamProfile, uint numberOfPiezoelectricsPerElements, uint[] elementsWithPiezoelectric, uint numberOfElements);
    }
}
