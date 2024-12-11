
echo "Building docker image..."
export DOCKER_CLI_HINTS=false
docker buildx build --platform linux/arm64 --network=host \
             -f ./Dockerfile . \
             --output type=local,dest=./results \
             -t cspice_container
echo "Docker image built successfully!";

# container_id=$(docker run -it --rm --detach cspice_container)
# docker cp ./cspice-fork $container_id:/workspace/
# docker exec -it $container_id sh -c "chmod +x ./makeall.csh && csh ./makeall.csh"
# docker cp $container_id:/workspace/cspice-fork ./
# docker stop $container_id