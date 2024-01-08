# Steps to follow in case you cannot connect to DSMLP

> ### Error like: "It seems like you might already have a GPU assigned to your account"

Deallocate the GPU using:

1. SSH into DSMLP using terminal and type the command 

```
kubectl get pods
```
2. Delete the pod using:

```
kubectl delete pod <pod_name>
```

> ### Error like: "launch-cse160-ssh.sh not found"

Replace launch-cse160-ssh.sh using with `/opt/dsmlp-course-launch-scripts/bin/launch-cse160-ssh.sh`

or

If you run "which launch-cse160-ssh.sh", you'll (hopefully) see `/opt/dsmlp-course-launch-scripts/bin/launch-cse160-ssh.sh` in the command line. If not, you'll need to add `/opt/dsmlp-course-launch-scripts/bin` to your path variable.

> ### Error like: "Failed to parse remote port from server output"

Try removing `dsmlp-login` and `vscode-dsmlp` from `/Users/rithvik/.ssh/known_hosts`
