﻿using IcVibracoes.Common.ErrorCodes;
using IcVibracoes.Core.AuxiliarOperations;
using IcVibracoes.Core.Calculator.Time;
using IcVibracoes.Core.DTO.NumericalMethodInput.RigidBody;
using IcVibracoes.Core.Models;
using IcVibracoes.Core.Models.BeamCharacteristics;
using IcVibracoes.Core.NumericalIntegrationMethods.RungeKuttaForthOrder.RigidBody_1DF;
using IcVibracoes.DataContracts.RigidBody.OneDegreeFreedom;
using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace IcVibracoes.Core.Operations.RigidBody.CalculateVibration.OneDegreeFreedom
{
    /// <summary>
    /// It is responsible to calculate the vibration for a rigid body with one degrees freedom case.
    /// </summary>
    public class CalculateVibrationToOneDegreeFreedom : CalculateVibration_RigidBody<OneDegreeFreedomRequest, OneDegreeFreedomRequestData, OneDegreeFreedomResponse, OneDegreeFreedomResponseData>, ICalculateVibrationToOneDegreeFreedom
    {
        /// <summary>
        /// Class constructor.
        /// </summary>
        /// <param name="auxiliarOperation"></param>
        /// <param name="rungeKutta"></param>
        /// <param name="time"></param>
        public CalculateVibrationToOneDegreeFreedom(
            IAuxiliarOperation auxiliarOperation,
            IRungeKuttaForthOrderMethod_1DF rungeKutta,
            ITime time)
            : base(auxiliarOperation, rungeKutta, time)
        { }

        /// <summary>
        /// Builds the input of differential equation of motion.
        /// </summary>
        /// <param name="requestData"></param>
        /// <returns></returns>
        public override Task<OneDegreeOfFreedomInput> CreateInput(OneDegreeFreedomRequestData requestData)
        {
            if (requestData == null || requestData.MechanicalProperties == null)
            {
                return null;
            }

            return Task.FromResult(new OneDegreeOfFreedomInput
            {
                AngularFrequency = requestData.InitialAngularFrequency,
                DampingRatio = requestData.DampingRatioList.FirstOrDefault(),
                Force = requestData.Force,
                ForceType = ForceTypeFactory.Create(requestData.ForceType),
                Stiffness = requestData.MechanicalProperties.Stiffness,
                Mass = requestData.MechanicalProperties.Mass
            });
        }

        /// <summary>
        /// Builds the vector with the initial conditions to analysis.
        /// </summary>
        /// <param name="requestData"></param>
        /// <returns></returns>
        public override Task<double[]> BuildInitialConditions(OneDegreeFreedomRequestData requestData)
        {
            return Task.FromResult(new double[Constant.NumberOfRigidBodyVariables_1DF]
            {
                requestData.InitialDisplacement,
                requestData.InitialVelocity
            });
        }

        /// <summary>
        /// Create a path to the files with the analysis solution.
        /// </summary>
        /// <param name="response"></param>
        /// <param name="requestData"></param>
        /// <param name="analysisType"></param>
        /// <param name="dampingRatio"></param>
        /// <param name="angularFrequency"></param>
        /// <returns></returns>
        public override Task<string> CreateSolutionPath(OneDegreeFreedomResponse response, OneDegreeFreedomRequestData requestData, string analysisType, double dampingRatio, double angularFrequency)
        {
            string previousPath = Path.GetDirectoryName(Directory.GetCurrentDirectory());

            string folderPath = Path.Combine(
                previousPath,
                $"Solutions/RigidBody/OneDegreeFreedom/m={requestData.MechanicalProperties.Mass}_k={requestData.MechanicalProperties.Stiffness}");

            string fileName = $"{analysisType.Trim()}_m={requestData.MechanicalProperties.Mass}_k={requestData.MechanicalProperties.Stiffness}_dampingRatio={dampingRatio}_w={Math.Round(angularFrequency, 2)}.csv";

            string path = Path.Combine(folderPath, fileName);

            Directory.CreateDirectory(folderPath);

            if (File.Exists(path))
            {
                response.AddError(ErrorCode.OperationError, $"File already exist in path: '{path}'.");
                return Task.FromResult<string>(null);
            }
            else
            {
                return Task.FromResult(path);
            }
        }
    }
}
