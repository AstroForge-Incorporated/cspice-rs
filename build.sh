
echo "Building docker image..."
export DOCKER_CLI_HINTS=false
docker buildx build --platform linux/arm64 --no-cache --network=host \
             -f ./Dockerfile . \
             -t cspice_container
echo "Docker image built successfully!";

container_id=$(docker run -it --rm --detach cspice_container)
# docker cp ./cspice-fork $container_id:/workspace/
# docker exec -it $container_id sh -c "chmod +x ./makeall.csh && csh ./makeall.csh"
docker cp $container_id:/workspace/cspice-fork ./results
docker stop $container_id