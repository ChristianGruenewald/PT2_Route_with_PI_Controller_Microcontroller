# PT2_Route_with_PI_Controller_Microcontroller
 
 Welcome!
 
 Realized circuit with the Atmel AVR Atmega 2560. It is a PT2 controlled system, which is later to be controlled with the Atmega in a PI controller.
 
 ## Route parameters
 
 |Name                |Parameter                         |Value                       |
|----------------|-------------------------------|-----------------------------|
|$$T_1$$|First time constant of the pt2 path           |`20 ms`          |
|$$T_2$$  | Second Time constant of the pt2 path            |`10 ms`            |
|$$V_S$$        |Gain of the PT2 route|`1`|
 
 ## circuit with the Atmel AVR Atmega 2560
 
 <img width="606" alt="image" src="https://user-images.githubusercontent.com/95079900/198340233-be049739-d353-47f0-a53f-bef25f40fa63.png">
 
 ## Calculation of the PI controller
The calculation of the PI controller with PT2 route is based on the dynamic compensation of the largest time constant (20 ms) and a subsequent pole specification.
#####  PT2- transfer function

$$
G(S)=V_s\cdot \frac{1}{ (T_1\cdot S+1)(T_2\cdot S+1) }
$$

### Dynamic Compensation of T1
With open controlled system F0:

$$
F_0(S)=\frac{V_R\cdot (T_N S+1)}{T_NS (T_1\cdot S+1)(T_2\cdot S+1) } 
$$

with 

$$ 
T_N=T_1
$$

T1 cancels out:

$$
F_0(S)=\frac{V_R\cdot (T_N S+1)}{T_NS (T_N\cdot S+1)(T_2\cdot S+1) } =\frac{V_R}{T_NS (T_2\cdot S+1) }
$$

### Pole Specifictation
Using the above function, the gain VR for the PI controller can be determined using the following formula:

$$
V_R=\frac{T_N}{4D^2T_2}
$$

Now the damping can be selected. This is usually selected with D=1, D=0.7 or D=0.5.

 For D=1:
 
 $$
 V_R=\frac{20 ms}{4\cdot 1^2\cdot 10 ms}\approx0.5
 $$
 
 For D=0.7:
 
 $$
 V_R=\frac{20 ms}{4\cdot 0.7^2\cdot 10 ms}\approx1
 $$
 
 For D=0.5: 
 
 $$
 V_R=\frac{20 ms}{4\cdot 0.5^2\cdot 10 ms}\approx2
 $$
 
 Now all cases have been examined and implemented in the controller.
 
 ## Result 
 With the parameters of the PT2 path selected here, the PI controller can control best with damping 1, since the best compromise can be selected here between dynamics, interference accuracy and control accuracy.

