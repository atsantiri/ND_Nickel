#!/usr/bin/env python3

import numpy as np
from scipy import constants
import matplotlib.pyplot as plt

class Isotope:

    def __init__(self, Z, A):

        self.Z = int(Z)
        self.A = int(A)
        self.N = A - Z

        # e_XL = energy of the XL resonance in MeV
        # gamma_XL = width of the XL resonance in MeV
        # sigma_XL = cross section ("strength") of the XL resonance in mb

        # E1 = Giant Dipole Resonance
        # Equations 173 and 174 of RIPL-3 
        # Equation 4.72 of TALYS User Manual 1.8
        self.e_E1 = 31.2 * pow(self.A, -1./3.) + 20.6 * pow(self.A, -1./6.) 
        self.gamma_E1 = 0.026 * pow(self.e_E1, 1.91)
        self.sigma_E1 = 1.2 * 120 * self.N * self.Z / (self.A * np.pi * self.gamma_E1)

        # E2 = Isoscalar Giant Resonance
        # Equation 4.73 of TALYS User Manual 1.8
        # There is a typo in equation 4.73: the resonance energy should be squared in the calculation of sigma
        # Also, instead of 0.00014, the value 1.5 * pow(10, -4.) is used to follow the convention of many others
        self.e_E2 = 63. * pow(self.A, -1./3.)
        self.gamma_E2 = 6.11 - 0.012 * self.A
        self.sigma_E2 = 1.5 * pow(10, -4.) * pow(self.Z * self.e_E2, 2.) / (pow(self.A, 1./3.) * self.gamma_E2)

        # M1 = Spin-Flip Giant Resonance
        # Below equation 189 of RIPL-3 
        # Equation 4.75 of TALYS User Manual 1.8
        self.e_M1 = 41. * pow(self.A, -1./3.)
        self.gamma_M1 = 4.
        # To find sigma_M1,
        # read comment below 4.75 and page 207 of TALYS User Manual 1.8, and
        # equations 188 and 189 of RIPL-3
        e_gamma_reference = 7
        factor_M1 = self.gamma_ray_strength_function('E', 1, e_gamma_reference) / (0.0588 * pow(A, 0.878))

        # Solve for sigma in whatever equation is being used for the E1 strength function

        # If the SLO is being used for E1, solve equation 4.67 of TALYS User Manual 1.8 for sigma
        '''self.sigma_M1 = (factor_M1 / Isotope.K(1)
                         * (pow(pow(e_gamma_reference, 2.) - pow(self.e_M1, 2.), 2.)
                            + pow(e_gamma_reference, 2.) * pow(self.gamma_M1, 2.))
                         / (e_gamma_reference * pow(self.gamma_M1, 2.)))'''

        # If the GLO/EGLO is being used for E1, solve equation 4.69 of TALYS User Manual 1.8 for sigma
        gamma_XL_damp = self.energy_dependent_damping_width(e_gamma_reference) # the function energy_dependent_damping_width uses E1 parameters, so below is an attempt to replace references to E1 with M1
        '''gamma_XL_damp = (self.gamma_M1 * ((pow(e_gamma_reference, 2.) + 4 * pow(np.pi, 2.) * pow(self.nuclear_temperature(), 2.))
                                          / pow(self.e_M1, 2.)))'''
        
        T = self.nuclear_temperature()
        self.sigma_M1 = (factor_M1 / Isotope.K(1)
                         / ((e_gamma_reference * gamma_XL_damp
                             / (pow(pow(e_gamma_reference, 2.) - pow(self.e_M1, 2.), 2.)
                                + pow(e_gamma_reference, 2.) * pow(gamma_XL_damp, 2.)))
                            + 0.7 * self.gamma_M1 * 4 * pow(np.pi, 2.) * pow(T, 2.)
                            / pow(self.e_M1, 5.)) # raised to the 5th power, not 3rd!
                         / self.gamma_M1)
        
    def __str__(self):
        output = 'Gamma-ray strength function parameters for (Z = {}, A = {})\n'.format(self.Z, self.A)
        output += 'E1 (Giant Dipole Resonance) Parameters\n'
        output += 'energy (MeV) = {}\n'.format(self.e_E1)
        output += 'gamma (MeV)  = {}\n'.format(self.gamma_E1)
        output += 'sigma (mb)   = {}\n'.format(self.sigma_E1)
        output += 'E2 (Isoscalar Giant Resonance) Parameters\n'
        output += 'energy (MeV) = {}\n'.format(self.e_E2)
        output += 'gamma (MeV)  = {}\n'.format(self.gamma_E2)
        output += 'sigma (mb)   = {}\n'.format(self.sigma_E2)
        output += 'M1 (Spin-Flip Giant Resonance) Parameters\n'
        output += 'energy (MeV) = {}\n'.format(self.e_M1)
        output += 'gamma (MeV)  = {}\n'.format(self.gamma_M1)
        output += 'sigma (mb)   = {}'.format(self.sigma_M1)
        return output

    def K(L):
        # Equation 4.68 of TALYS User Manual 1.8
        K = pow(2 * L + 1, -1.) * pow(np.pi * constants.hbar * constants.speed_of_light, -2.)
        # Must convert units of K to match units in other parts of the formula for the strength function!
        # Above, K has units of (joules**2 * meters**2)**-1
        # Need to convert joules**2 to MeV**2 and meters**2 to millibarns
        K = K * pow(3.8956436e25, -1.) * (1e-31)
        return K

    def energy_dependent_damping_width(self, Egamma):
        # equation 4.70 of TALYS User Manual 1.8
        return (self.gamma_E1 * ((pow(Egamma, 2.) + 4 * pow(np.pi, 2.) * pow(self.nuclear_temperature(), 2.))
                                 / pow(self.e_E1, 2.)))
        
    def nuclear_temperature(self):
        # equation 4.71 of TALYS User Manual 1.8
        return 0.5 # assuming constant temperature right now
    
    def gamma_ray_strength_function(self, X, L, Egamma):

        if X not in ('E','M'):
            raise ValueError("electromagnetic type must be either 'E' or 'M'")
        if not isinstance(L, int):
            raise TypeError('multipolarity must be an integer')
        if L < 1:
            raise ValueError('multipolarity must be >= 1')
            
        e_XL = 0
        gamma_XL = 0
        sigma_XL = 0

        if X == 'E':
            if L == 1:
                e_XL = self.e_E1
                gamma_XL = self.gamma_E1
                sigma_XL = self.sigma_E1
            elif L > 1:
                e_XL = self.e_E2
                gamma_XL = self.gamma_E2
                sigma_XL = self.sigma_E2
                # If this is an E2 transition, no more needs to be done.
                # Otherwise, compute the resonance strength iteratively using
                # the prescription of equation 4.74 of TALYS User Manual 1.8
                for i in range(2, L):
                    sigma_XL *= pow(8.10, -4.)
        elif X == 'M':
            e_XL = self.e_M1
            gamma_XL = self.gamma_M1
            sigma_XL = self.sigma_M1
            # If this is an M1 transition, no more needs to be done.
            # Otherwise, compute the resonance strength iteratively using
            # the prescription of equation 4.76 of TALYS User Manual 1.8
            for i in range(1, L):
                sigma_XL *= pow(8.10, -4.)

        # Now that we have the parameters, calculate the strength function

        if X == 'E' and L == 1:
            # enhanced generalized Lorentzian (EGLO) / generalized Lorentzian (GLO)
            # Equation 140 of RIPL-3 (referred to as EGLO)
            # Equation 4.69 of TALYS User Manual 1.8 (referred to as GLO)
            # There is a typo in equation 4.69: one of the resonance energies should be raised to the 5th power
            gamma_XL_damp = self.energy_dependent_damping_width(Egamma)
            T = self.nuclear_temperature()
            return (Isotope.K(L) * ((Egamma * gamma_XL_damp
                                     / (pow(pow(Egamma, 2.) - pow(e_XL, 2.), 2.)
                                        + pow(Egamma, 2.) * pow(gamma_XL_damp, 2.)))
                                    + 0.7 * gamma_XL * 4 * pow(np.pi, 2.) * pow(T, 2.)
                                    / pow(e_XL, 5.)) # raised to the 5th power, not 3rd!
                    * sigma_XL * gamma_XL)
        
        else:
            # standard Lorentzian (SLO)
            # equation 135 and 174 of RIPL-3 
            # equation 4.67 of TALYS User Manual 1.8
            return (Isotope.K(L) * sigma_XL * Egamma * pow(gamma_XL, 2.)
                    / (pow(pow(Egamma, 2.) - pow(e_XL, 2.), 2.) + pow(Egamma, 2.) * pow(gamma_XL, 2.)))
        
if __name__ == '__main__':

    gamma_ray_energies = np.linspace(0, 100, num=2000)

    Z = input('Z = ')
    A = input('A = ')
    isotope = Isotope(int(Z), int(A))

    print(isotope)
    
    fig, axes = plt.subplots()
    axes.plot(gamma_ray_energies, isotope.gamma_ray_strength_function(X = 'E', L = 1, Egamma = gamma_ray_energies), label = 'E1')
    axes.plot(gamma_ray_energies, isotope.gamma_ray_strength_function(X = 'E', L = 2, Egamma = gamma_ray_energies), label = 'E2')
    axes.plot(gamma_ray_energies, isotope.gamma_ray_strength_function(X = 'M', L = 1, Egamma = gamma_ray_energies), label = 'M1')
    axes.set_yscale('log')
    axes.legend(loc = 'best')
    plt.show()


    
    


    
