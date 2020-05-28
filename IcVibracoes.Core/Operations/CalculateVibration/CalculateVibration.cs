﻿using IcVibracoes.Core.DTO.NumericalMethodInput;
using IcVibracoes.DataContracts;
using System.Threading.Tasks;

namespace IcVibracoes.Core.Operations.CalculateVibration
{
    /// <summary>
    /// It's responsible to calculate the vibration to a structure.
    /// </summary>
    /// <typeparam name="TRequest"></typeparam>
    /// <typeparam name="TRequestData"></typeparam>
    /// <typeparam name="TResponse"></typeparam>
    /// <typeparam name="TResponseData"></typeparam>
    public abstract class CalculateVibration<TRequest, TRequestData, TResponse, TResponseData, TInput> : OperationBase<TRequest, TRequestData, TResponse, TResponseData>, ICalculateVibration<TRequest, TRequestData, TResponse, TResponseData, TInput>
        where TRequest : OperationRequestBase<TRequestData>
        where TRequestData : OperationRequestData
        where TResponse : OperationResponseBase<TResponseData>, new()
        where TResponseData : OperationResponseData
        where TInput : NumericalMethodInput, new()
    {
        /// <summary>
        /// Calculates the input to newmark integration method.
        /// </summary>
        /// <param name="request"></param>
        /// <param name="degreesOfFreedom"></param>
        /// <returns></returns>
        public abstract Task<TInput> CreateInput(TRequest request, uint degreesOfFreedom);

        /// <summary>
        /// Creates the file path to write the results.
        /// </summary>
        /// <param name="analysisType"></param>
        /// <param name="input"></param>
        /// <param name="response"></param>
        /// <returns></returns>
        public abstract Task<string> CreateSolutionPath(string analysisType, TInput input, TResponse response);

        /// <summary>
        /// Creates the file path to write the maximum values calculated in the analysis.
        /// </summary>
        /// <param name="analysisType"></param>
        /// <param name="input"></param>
        /// <param name="response"></param>
        /// <returns></returns>
        public abstract Task<string> CreateMaxValuesPath(string analysisType, TInput input, TResponse response);
    }
}
