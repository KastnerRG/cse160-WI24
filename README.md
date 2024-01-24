# cse160-WI24
CSE 160 Programming Assignments - Winter 24

## Access DSMLP using ssh

You will use the [UCSD Data Science / Machine Learning Platform (DSMLP)](https://support.ucsd.edu/its?id=kb_article_view&sys_kb_id=fda9846287908954947a0fa8cebb352b) to get access to a GPU. 

You can login to DSMLP using by `ssh USERNAME@dsmlp-login.ucsd.edu`. Your username and password are the same as your UCSD account. You can set up an [ssh key](https://support.ucsd.edu/services?id=kb_article_view&sys_kb_id=711d8e9e1b7b34d473462fc4604bcb47) that allows you to more easily login. 

Follow the steps in this [link](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent?platform=mac#about-ssh-key-passphrases) to generate a key pair (Follow the guide till step 3 under 'Adding your SSH key to the ssh-agent').

DSMLP uses containers to set up its software environment. You must create a container that provides access to a GPU with CUDA installed using the command ` launch.sh -g 1 -s -i ghcr.io/ucsd-ets/nvcr-cuda:main -W CSE160_WI24_A00`

Once you have that container, you can compile and run the Makefiles in the PA directories.

Please be considerate on your use of the GPUs. The GPUs in DSMLP are shared within this class and across campus. If you are not actively using the GPU, you should shut down the container to allow others to access it.


## Access DSMLP using VSCode

It is possible to access DSMLP using a local version of VSCode. 

Steps:

1. Create an SSH Key on your local machine and append the public key to DSMLP's ~/.ssh/authorized_keys file. Follow the steps in this [link](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent?platform=mac#about-ssh-key-passphrases) to generate a key pair (Follow the guide till step 3 under 'Adding your SSH key to the ssh-agent').
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
ProxyCommand ssh -i 'path to local private key' 'active directory username'@dsmlp-login.ucsd.edu launch-cse160-ssh-wi24.sh -g 1 -p normal -P Always
```

7. Click on the >< key at the bottom left corner and then click on Connect to Host ... You should see a vscode-dsmlp option. Click on it to start your session. 

A video is attached in case there are any issues with following the steps:

https://github.com/KastnerRG/cse160-WI24/assets/114721561/0829b8bd-96a6-48f7-b968-9ee9eda30c51


You already have access to GPU infrastructure on DSMLP; i.e. it starts a container with GPU access and loads it with a software image that contains CUDA and other basic packages. 

You must be within GPU container in order to properly compile. If you get an error about not having access to nvcc, then you are not in the container.

Please only use the container when you are compiling and release it when you are completed.

## Note
When you close VSCode, the kubernetes pod is not released automatically. You have to manually delete the pod using `kubectl delete pod <pod_name>`.
To find your pod's name, you can run `kubectl get pods` and find all the pods open. Once you do this, you will be able to use the terminal for launching a new container if you want.

P.S. Watch the first Discussion lecture for a step-by-step guide for setting up SSH key pairs and vs code.
