/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/19 14:03:55 by omiroshn          #+#    #+#             */
/*   Updated: 2018/03/19 14:03:56 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_cl_error(t_map *m)
{
	size_t	log_size;
	char	*log_msg;

	clGetProgramBuildInfo(m->cl.program, m->cl.device, CL_PROGRAM_BUILD_LOG, 0,
													NULL, &log_size);
	log_msg = (char *)malloc(log_size);
	clGetProgramBuildInfo(m->cl.program, m->cl.device, CL_PROGRAM_BUILD_LOG,
													log_size, log_msg, NULL);
	ft_putendl(log_msg);
	ft_memdel((void **)&log_msg);
	put_error("clBuildProgram Failed");
}

void	kernel_create(t_map *m, cl_int ret, char *str, size_t size)
{
	m->cl.image = clCreateBuffer(m->cl.context, CL_MEM_READ_WRITE,
		m->screen->h * m->screen->w * sizeof(int), NULL, &ret);
	ret ? put_error("clCreateBuffer Failed") : 0;
	m->cl.objects = clCreateBuffer(m->cl.context, CL_MEM_USE_HOST_PTR,
		sizeof(t_geom) * m->geom.objects, m->geom.obj, &ret);
	ret ? put_error("clCreateBuffer Failed") : 0;
	m->cl.lights = clCreateBuffer(m->cl.context, CL_MEM_USE_HOST_PTR,
		sizeof(t_geom) * m->geom.lights, m->geom.light, &ret);
	ret ? put_error("clCreateBuffer Failed") : 0;
	m->cl.program = clCreateProgramWithSource(m->cl.context, 1,
		(const char **)&str, (const size_t *)&size, &ret);
	ret ? put_error("clCreateProgramWithSource Failed") : 0;
	ret = clBuildProgram(m->cl.program, 1, &m->cl.device, NULL, NULL, NULL);
	ret ? put_cl_error(m) : 0;
	m->cl.kernel = clCreateKernel(m->cl.program, "draw", &ret);
	ret ? put_error("clCreateKernel Failed") : 0;
	free(str);
}

void	kernel_init(t_map *m)
{
	int		fp;
	char	*str;
	cl_int	ret;
	size_t	size;

	(fp = open("./kernel/kernel.cl", O_RDONLY)) <= 0
		? put_error("Failed to load kernel.") : 0;
	!(str = (char*)malloc(MAX_SOURCE_SIZE))
		? put_error("Can't allocate memory.") : 0;
	(size = read(fp, str, MAX_SOURCE_SIZE)) <= 0
		? put_error("Can't read file.") : 0;
	close(fp);
	ret = clGetPlatformIDs(1, &m->cl.platform_id, &m->cl.ret_num_platforms);
	ret ? put_error("clGetPlatformIDs Failed") : 0;
	ret = clGetDeviceIDs(m->cl.platform_id, CL_DEVICE_TYPE_DEFAULT,
		1, &m->cl.device, &m->cl.ret_num_devices);
	ret ? put_error("clGetDeviceIDs Failed") : 0;
	m->cl.context = clCreateContext(NULL, 1, &m->cl.device, NULL, NULL, &ret);
	ret ? put_error("clCreateContext Failed") : 0;
	m->cl.queue = clCreateCommandQueue(m->cl.context, m->cl.device, 0, &ret);
	ret ? put_error("clCreateCommandQueue Failed") : 0;
	kernel_create(m, ret, str, size);
}

void	kernel(t_map *m)
{
	cl_int				ret;

	(ret = clSetKernelArg(m->cl.kernel, 0, sizeof(cl_mem),
	(void *)&m->cl.image)) ? put_error("Can't set parameter") : 0;
	(ret = clSetKernelArg(m->cl.kernel, 1, sizeof(t_geom),
	(void *)&m->geom)) ? put_error("Can't set parameter") : 0;
	(ret = clSetKernelArg(m->cl.kernel, 2, sizeof(cl_mem),
	(void *)&m->cl.objects)) ? put_error("Can't set parameter") : 0;
	(ret = clSetKernelArg(m->cl.kernel, 3, sizeof(cl_mem),
	(void *)&m->cl.lights)) ? put_error("Can't set parameter") : 0;
	ret = clEnqueueNDRangeKernel(m->cl.queue, m->cl.kernel, 2, NULL,
		(size_t[3]){m->screen->w, m->screen->h, 0}, NULL, 0, NULL, NULL);
	ret ? put_error("clEnqueueNDRangeKernel Failed") : 0;
	ret = clEnqueueReadBuffer(m->cl.queue, m->cl.image, CL_TRUE, 0,
		m->screen->w * m->screen->h * sizeof(int), (int*)m->screen->pixels,
		0, NULL, NULL);
	ret ? put_error("clEnqueueReadBuffer Failed") : 0;
}
