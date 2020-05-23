﻿using IcVibracoes.Common.Profiles;

namespace IcVibracoes.DataContracts.FiniteElements.BeamWithPiezoelectric
{
    /// <summary>
    /// It represents the request content of CalculatePiezoelectric operations.
    /// </summary>
    /// <typeparam name="TProfile"></typeparam>
    public class BeamWithPiezoelectricRequest<TProfile> : FiniteElementsRequest<TProfile, PiezoelectricRequestData<TProfile>>
        where TProfile : Profile, new()
    {
        /// <summary>
        /// The analysis type. 
        /// </summary>
        public override string AnalysisType 
        {
            get
            {
                return "FiniteElements_BeamWithPiezoelectric";
            }
        }
    }
}
