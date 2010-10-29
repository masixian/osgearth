/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2008-2010 Pelican Mapping
 * http://osgearth.org
 *
 * osgEarth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <osgEarth/TerrainOptions>
#include <osg/Notify>
#include <OpenThreads/Thread>

using namespace osgEarth;

//------------------------------------------------------------------------

LoadingPolicy::LoadingPolicy( const Config& conf ) :
_mode( MODE_STANDARD ),
_numLoadingThreads( 4 ),
_numLoadingThreadsPerCore( 4 ),
_numCompileThreads( 2 ),
_numCompileThreadsPerCore( 0.5 )
{
    fromConfig( conf );
}

void
LoadingPolicy::fromConfig( const Config& conf )
{
    conf.getIfSet( "mode", "standard", _mode, MODE_STANDARD );
    conf.getIfSet( "mode", "sequential", _mode, MODE_SEQUENTIAL );
    conf.getIfSet( "mode", "preemptive", _mode, MODE_PREEMPTIVE );
    conf.getIfSet( "loading_threads", _numLoadingThreads );
    conf.getIfSet( "loading_threads_per_logical_processor", _numLoadingThreadsPerCore );
    conf.getIfSet( "loading_threads_per_core", _numLoadingThreadsPerCore );
    conf.getIfSet( "compile_threads", _numCompileThreads );
    conf.getIfSet( "compile_threads_per_core", _numCompileThreadsPerCore );
}

Config
LoadingPolicy::getConfig() const
{
    Config conf( "loading_policy" );
    conf.addIfSet( "mode", "standard", _mode, MODE_STANDARD );
    conf.addIfSet( "mode", "sequential", _mode, MODE_SEQUENTIAL );
    conf.addIfSet( "mode", "preemptive", _mode, MODE_PREEMPTIVE );
    conf.addIfSet( "loading_threads", _numLoadingThreads );
    conf.addIfSet( "loading_threads_per_core", _numLoadingThreadsPerCore );
    conf.addIfSet( "compile_threads", _numCompileThreads );
    conf.addIfSet( "compile_threads_per_core", _numCompileThreadsPerCore );
    return conf;
}

//----------------------------------------------------------------------------

TerrainOptions::TerrainOptions( const ConfigOptions& options ) :
DriverConfigOptions( options ),
_loadingPolicy( LoadingPolicy() ),
_verticalScale( 1.0f ),
_heightFieldSampleRatio( 1.0f ),
_minTileRangeFactor( 6.0 ),
_normalizeEdges( false ),
_combineLayers( true ),
_maxLOD( 23 ),
//_layeringTechnique( LAYERING_COMPOSITE ),
_compositingTech( COMPOSITING_AUTO ),
_enableLighting( false ),
_elevationInterpolation( INTERP_BILINEAR ),
_attenuationDistance( 1000000 )
{
    fromConfig( _conf );
}

Config
TerrainOptions::getConfig() const
{
    Config conf = DriverConfigOptions::getConfig();
    conf.key() = "terrain";

    conf.updateObjIfSet( "loading_policy", _loadingPolicy );
    conf.updateIfSet( "vertical_scale", _verticalScale );
    conf.updateIfSet( "sample_ratio", _heightFieldSampleRatio );
    conf.updateIfSet( "min_tile_range_factor", _minTileRangeFactor );
    conf.updateIfSet( "normalize_edges", _normalizeEdges );
    conf.updateIfSet( "combine_layers", _combineLayers );
    conf.updateIfSet( "max_lod", _maxLOD );
    conf.updateIfSet( "lighting", _enableLighting );
    conf.updateIfSet( "attenuation_distance", _attenuationDistance );

    conf.updateIfSet( "compositor", "auto",             _compositingTech, COMPOSITING_AUTO );
    conf.updateIfSet( "compositor", "texture_array",    _compositingTech, COMPOSITING_TEXTURE_ARRAY );
    conf.updateIfSet( "compositor", "multitexture",     _compositingTech, COMPOSITING_MULTITEXTURE_GPU );
    conf.updateIfSet( "compositor", "multipass",        _compositingTech, COMPOSITING_MULTIPASS );

    conf.updateIfSet( "elevation_interpolation", "nearest",     _elevationInterpolation, INTERP_NEAREST);
    conf.updateIfSet( "elevation_interpolation", "average",     _elevationInterpolation, INTERP_AVERAGE);
    conf.updateIfSet( "elevation_interpolation", "bilinear",    _elevationInterpolation, INTERP_BILINEAR);
    conf.updateIfSet( "elevation_interpolation", "triangulate", _elevationInterpolation, INTERP_TRIANGULATE);

    return conf;
}

void
TerrainOptions::fromConfig( const Config& conf )
{
    conf.getObjIfSet( "loading_policy", _loadingPolicy );
    conf.getIfSet( "vertical_scale", _verticalScale );
    conf.getIfSet( "sample_ratio", _heightFieldSampleRatio );
    conf.getIfSet( "min_tile_range_factor", _minTileRangeFactor );
    conf.getIfSet( "normalize_edges", _normalizeEdges );
    conf.getIfSet( "combine_layers", _combineLayers );
    conf.getIfSet( "max_lod", _maxLOD );
    conf.getIfSet( "lighting", _enableLighting );
    conf.getIfSet( "attenuation_distance", _attenuationDistance );

    conf.getIfSet( "compositor", "auto",             _compositingTech, COMPOSITING_AUTO );
    conf.getIfSet( "compositor", "texture_array",    _compositingTech, COMPOSITING_TEXTURE_ARRAY );
    conf.getIfSet( "compositor", "multitexture",     _compositingTech, COMPOSITING_MULTITEXTURE_GPU );
    conf.getIfSet( "compositor", "multitexture_gpu", _compositingTech, COMPOSITING_MULTITEXTURE_GPU );
    conf.getIfSet( "compositor", "multipass",        _compositingTech, COMPOSITING_MULTIPASS );

    conf.getIfSet( "elevation_interpolation", "nearest",     _elevationInterpolation, INTERP_NEAREST);
    conf.getIfSet( "elevation_interpolation", "average",     _elevationInterpolation, INTERP_AVERAGE);
    conf.getIfSet( "elevation_interpolation", "bilinear",    _elevationInterpolation, INTERP_BILINEAR);
    conf.getIfSet( "elevation_interpolation", "triangulate", _elevationInterpolation, INTERP_TRIANGULATE);
}
