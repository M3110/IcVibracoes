﻿using IcVibracoes.Common.Classes;
using IcVibracoes.Common.Profiles;
using System.Collections.Generic;

namespace IcVibracoes.DataContracts.FiniteElements
{
    /// <summary>
    /// It represents the 'data' content of Finite Elements request operation.
    /// </summary>
    /// <typeparam name="TProfile"></typeparam>
    public class FiniteElementsRequestData<TProfile> : OperationRequestData
        where TProfile : Profile
    {
        /// <summary>
        /// Number of elements in the beam.
        /// </summary>
        public uint NumberOfElements { get; set; }

        /// <summary>
        /// Beam material.
        /// </summary>
        public string Material { get; set; }

        /// <summary>
        /// Beam length.
        /// </summary>
        public double Length { get; set; }

        /// <summary>
        /// The beam fastenings.
        /// </summary>
        public List<Fastening> Fastenings { get; set; }

        /// <summary>
        /// Applied forces in the beam.
        /// </summary>
        public List<Force> Forces { get; set; }

        /// <summary>
        /// Beam profile.
        /// </summary>
        public TProfile Profile { get; set; }
    }
}