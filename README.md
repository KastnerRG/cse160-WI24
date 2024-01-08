# cse160-WI24
CSE 160 Programming Assignments - Winter 24

## Access DSMLP using ssh

You will use the [UCSD Data Science / Machine Learning Platform (DSMLP)](https://support.ucsd.edu/its?id=kb_article_view&sys_kb_id=fda9846287908954947a0fa8cebb352b) to get access to a GPU. 

You can login to DSMLP using by `ssh USERNAME@dsmlp-login.ucsd.edu`. Your username and password are your UCSD account. You can set up an [ssh key](https://support.ucsd.edu/services?id=kb_article_view&sys_kb_id=711d8e9e1b7b34d473462fc4604bcb47) that allows you to more easily login. 

DSMLP uses containers to set up its software environment. You must create a container that provides access to a GPU with CUDA installed using the command ` launch.sh -g 1 -s -i ucsdets/nvcr-cuda:latest -W CSE160_WI23_A00`

Once you have that container, you can compile and run the Makefiles in the PA directories.

Please be considerate on your use of the GPUs. The GPUs in DSMLP are shared within this class and across campus. If you are not actively using the GPU, you should shut down the container to allow others to access it.


## Access DSMLP using VSCode

It is possible to access DSMLP using a local version of VSCode. 

Steps:

1. Create an SSH Key on your local machine and append the public key to DSMLP's ~/.ssh/authorized_keys file
2. Install VS Code https://code.visualstudio.com/download
3. Install Remote-SSH plugin by searching for it in the extensions view
4. Click on the indicator on the bottom left corner

![image](https://user-images.githubusercontent.com/43923184/148268541-202b9806-7d08-415b-ad4d-7b4d04916388.png)

5. Click on Open SSH Configuration File ... and select any configuration file
6. Add the following lines:

```
Host vscode-dsmlp
HostName dsmlp-login.ucsd.edu
HostKeyAlias vscode-dsmlp
IdentitiesOnly yes
User 'active directory username'
ProxyCommand ssh -i 'path to local private key' 'active directory username'@dsmlp-login.ucsd.edu launch-cse160-ssh.sh -g 1 -p normal
```

7. Click on the >< key at the bottom left corner and then click on Connect to Host ... You should see a vscode-dsmlp option. Click on it to start your session. 


You already have access to GPU infrastructure on DSMLP; i.e. it starts a container with GPU access and loads it with a software image that contains CUDA and other basic packages. 

You must be within GPU container in order to properly compile. If you get an error about not having access to nvcc, then you are not in the container.

Please only use the container when you are compiling and release it when you are completed. 
